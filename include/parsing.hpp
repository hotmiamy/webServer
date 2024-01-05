#pragma once

#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "DirectiveHandler.hpp"
#include "ServerConfig.hpp"

struct IsNotSpace {
    bool operator()(int ch) const { return !std::isspace(ch); }
};

struct IsConsecutiveSpace {
    bool operator()(char a, char b) const {
        return std::isspace(a) && std::isspace(b);
    }
};

std::vector<ServerConfig> parse(std::ifstream &ifs);
