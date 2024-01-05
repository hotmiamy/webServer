#pragma once

#include <sstream>

#include "ServerConfig.hpp"

std::vector<ServerConfig> parse(std::ifstream &ifs);
