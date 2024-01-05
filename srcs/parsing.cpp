
#include "parsing.hpp"

std::vector<ServerConfig> parse(std::ifstream &ifs) {
    std::stringstream ss;
    ss << ifs.rdbuf();
    std::string fileContent = ss.str();
    // ...
    std::vector<ServerConfig> configs;
    return configs;
}
