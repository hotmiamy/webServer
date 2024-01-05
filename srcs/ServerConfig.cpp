#include "ServerConfig.hpp"

#include "parsing.hpp"

ServerConfig::ServerConfig()
    : _port(), _serverNames(), _errorPages(), _locations() {}

ServerConfig::ServerConfig(const ServerConfig &other) { *this = other; }

ServerConfig &ServerConfig::operator=(const ServerConfig &other) {
    if (this != &other) {
    }
    return *this;
}

ServerConfig::~ServerConfig() {}

std::vector<ServerConfig> ServerConfig::fromFile(const std::string &file) {
    std::ifstream ifs(file.c_str());

    if (!ifs.is_open()) {
        throw std::invalid_argument("invalid file: " + file);
    }

    return parse(ifs);
}

void ServerConfig::setPort(int port) { _port = port; }

void ServerConfig::addErrorPage(int errorCode, const std::string &path) {
    _errorPages[errorCode] = path;
}

void ServerConfig::addServer(const std::string &server) {
    _serverNames.push_back(server);
}
