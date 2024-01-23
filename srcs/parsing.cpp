#include "parsing.hpp"

static ServerConfig serverBlockToServerConfig(const std::string &block);

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
            throw std::runtime_error("");
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
        throw std::runtime_error("");
    }

    std::vector<std::string> serverConfigBlocks =
        splitServerConfigBlocks(fileContent);
    if (serverConfigBlocks.empty()) {
        throw std::runtime_error("");
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
    std::string res = str;
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

static ServerConfig serverBlockToServerConfig(const std::string &block) {
static ServerConfig serverBlockToServerConfig(std::string &block) {
    trimWhitespaces(block);
    std::istringstream iss(block);
    std::string line;

    ServerConfig config;
    DirectiveHandler handler;

    while (std::getline(iss, line)) {
        if (line.empty()) {
            continue;
        }

        std::istringstream lineIss(line);
        std::string directive;
        lineIss >> directive;

        handler.process(directive, lineIss, config);
    }
    return config;
}
