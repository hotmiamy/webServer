#include "ServerConfig.hpp"

#include <iostream>

ServerConfig::ServerConfig() {}

ServerConfig::ServerConfig(const ServerConfig &other) { *this = other; }

ServerConfig &ServerConfig::operator=(const ServerConfig &other) {
    if (this != &other) {
    }
    return *this;
}

ServerConfig::~ServerConfig() {}

ServerConfig ServerConfig::fromFile(const std::string &file) {
    std::ifstream ifs(file.c_str());

    if (!ifs.is_open()) {
        throw std::invalid_argument("invalid file: " + file);
    }

    ServerConfig serverConfig;
    serverConfig.parse(ifs);
    return serverConfig;
}

void ServerConfig::parse(std::ifstream &ifs) {
    std::string line;
    while (std::getline(ifs, line)) {
        std::cout << line << '\n';
    }
}
