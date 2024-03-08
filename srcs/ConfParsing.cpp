#include "ConfParsing.hpp"

static ServerConfig serverBlockToServerConfig(std::string &block);

static bool hasCorrectAmountOfBrackets(const std::string &str) {
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

static std::vector<std::string> splitServerConfigBlocks(const std::string &s) {
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

    std::vector<ServerConfig> configs;
    for (std::string::size_type i = 0; i < serverConfigBlocks.size(); ++i) {
        ServerConfig config = serverBlockToServerConfig(serverConfigBlocks[i]);
        configs.push_back(config);
    }
    return configs;
}

static std::string removeConsecutiveWhitespaces(const std::string &str) {
    if (str.empty()) {
        return "";
    }
    std::string res(str);
    res.erase(std::unique(res.begin(), res.end(), IsConsecutiveSpace()),
              res.end());
    return res;
}

static void trimWhitespaces(std::string &line) {
    // respectively: remove leading whitespaces from start, end, and between the
    // string
    line.erase(line.begin(),
               std::find_if(line.begin(), line.end(), IsNotSpace()));
    line.erase(std::find_if(line.rbegin(), line.rend(), IsNotSpace()).base(),
               line.end());
    line = removeConsecutiveWhitespaces(line);
}

static std::string getLocationBlock(std::istringstream &iss,
                                    std::istringstream &lineIss) {
    std::string locationBlock, line;
    while (std::getline(iss, line) && line.find("}") == std::string::npos) {
        locationBlock += line + '\n';
    }
    locationBlock = lineIss.str() + '\n' + locationBlock + '}';
    locationBlock.erase(0, locationBlock.find_first_not_of("location "));
    return locationBlock;
}

static ServerConfig serverBlockToServerConfig(std::string &block) {
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
    if (cfg.good()) {
        return cfg;
    }
    throw std::runtime_error("bad 'server' block: " + block);
}
