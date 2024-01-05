#pragma once

#include <cstring>
#include <sstream>
#include <string>

#include "ServerConfig.hpp"

std::vector<ServerConfig> parse(std::ifstream &ifs);
