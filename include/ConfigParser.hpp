#pragma once

#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "DirectiveHandler.hpp"
#include "ServerConfig.hpp"

namespace ConfigParser {

struct IsNotSpace {
    bool operator()(int ch) const;
};

struct IsConsecutiveSpace {
    bool operator()(char a, char b) const;
};

bool hasCorrectAmountOfBrackets(const std::string &str);

std::vector<std::string> splitServerConfigBlocks(const std::string &s);

std::string removeConsecutiveWhitespaces(const std::string &str);

void trimWhitespaces(std::string &line);

std::string getLocationBlock(std::istringstream &iss,
                             std::istringstream &lineIss);

ServerConfig serverBlockToServerConfig(std::string &block);

std::vector<ServerConfig> parse(std::ifstream &ifs);

}  // namespace ConfigParser
