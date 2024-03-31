#include "ConfigParser.hpp"

namespace ConfigParser {

bool IsNotSpace::operator()(int ch) const { return !std::isspace(ch); }

bool IsConsecutiveSpace::operator()(char a, char b) const {
    return std::isspace(a) && std::isspace(b);
}

std::vector<ServerConfig> parse(std::ifstream &ifs) {
    std::stringstream ss;
    ss << ifs.rdbuf();
    std::string fileContent = ss.str();

    if (!hasCorrectAmountOfBrackets(fileContent)) {
        throw std::runtime_error("there are some missing brackets");
    }

    std::vector<std::string> serverConfigBlocks =
        splitServerConfigBlocks(fileContent);
    if (serverConfigBlocks.empty()) {
        throw std::runtime_error(
            "a config file must contain at least one server block");
    }

    ServerVec configs;
    for (std::string::size_type i = 0; i < serverConfigBlocks.size(); ++i) {
        ServerConfig config = serverBlockToServerConfig(serverConfigBlocks[i]);
        configs.push_back(config);
    }

    std::string default_server_root = configs.begin()->getRoot();
    for (ServerVec::iterator it = configs.begin() + 1; it != configs.end();
         ++it) {
        it->setRoot(default_server_root);
    }

    std::set<std::string> set;
    for (ServerVec::const_iterator it = configs.begin(); it != configs.end();
         ++it) {
        std::string port = it->getPort();
        if (set.count(port) > 0) {
            throw std::runtime_error(
                "there are 2 or more servers with the same port: " + port);
        }
        set.insert(port);
    }

    return configs;
}

bool hasCorrectAmountOfBrackets(const std::string &str) {
    int left = 0, right = 0;

    for (size_t i = 0; i < str.size(); ++i) {
        char c = str[i];
        if (c == '{') {
            left++;
            continue;
        }
        if (c == '}') {
            right++;
        }
    }
    return left == right;
}

std::vector<std::string> splitServerConfigBlocks(const std::string &s) {
    const char *directive = "server {";
    std::string str(s);
    std::vector<std::string> blocks;

    std::string::size_type pos;
    while ((pos = str.find("server ", 0)) != str.npos) {
        std::string::size_type openingBracketPos = str.find("{", pos);
        if (str.substr(pos, strlen(directive)) != directive) {
            throw std::runtime_error(
                "'server' block must be closed, check the config file");
        }
        std::string::size_type serverBlockEnd =
            str.rfind("}", str.find(directive, openingBracketPos));
        std::string::size_type blockSize = (serverBlockEnd + 1) - pos;
        blocks.push_back(str.substr(pos, blockSize));
        str.erase(pos, blockSize);
    }
    return blocks;
}

std::string removeConsecutiveWhitespaces(const std::string &str) {
    if (str.empty()) {
        return "";
    }
    std::string res(str);
    res.erase(std::unique(res.begin(), res.end(), IsConsecutiveSpace()),
              res.end());
    return res;
}

void trimWhitespaces(std::string &line) {
    line.erase(line.begin(),
               std::find_if(line.begin(), line.end(), IsNotSpace()));
    line.erase(std::find_if(line.rbegin(), line.rend(), IsNotSpace()).base(),
               line.end());
    line = removeConsecutiveWhitespaces(line);
}

std::string getLocationBlock(std::istringstream &iss,
                             std::istringstream &lineIss) {
    std::string locationBlock, line;
    while (std::getline(iss, line) && line.find("}") == std::string::npos) {
        locationBlock += line + '\n';
    }
    locationBlock = lineIss.str() + '\n' + locationBlock + '}';
    locationBlock.erase(0, locationBlock.find_first_not_of("location "));
    return locationBlock;
}

ServerConfig serverBlockToServerConfig(std::string &block) {
    trimWhitespaces(block);
    std::istringstream iss(block);
    std::string line;

    DirectiveHandler handler;
    while (std::getline(iss, line)) {
        if (line.empty()) {
            continue;
        }

        std::istringstream lineIss(line);
        std::string directive;
        lineIss >> directive;

        if (directive == "location") {
            std::istringstream locationIss(getLocationBlock(iss, lineIss));
            handler.process(directive, locationIss);
            continue;
        }

        handler.process(directive, lineIss);
    }

    ServerConfig cfg = handler.getCfg();
    cfg.setErrorPages(ServerUtils::getDefaultErrorPages(cfg.getErrorPages()));
    if (cfg.good()) {
        return cfg;
    }
    throw std::runtime_error("bad 'server' block: " + block);
}

}  // namespace ConfigParser
