#include "parsing.hpp"

#include <iostream>

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
    return configs;
}

// static std::vector<ServerConfig> serverBlockToServerConfig(
//     const std::vector<std::string> &blocks) {}
