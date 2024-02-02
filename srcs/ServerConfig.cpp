#include "ServerConfig.hpp"

#include "parsing.hpp"

ServerConfig::ServerConfig()
    : _port(), _root(), _serverNames(), _errorPages(), _locations() {}

ServerConfig::ServerConfig(const ServerConfig &other) { *this = other; }

ServerConfig &ServerConfig::operator=(const ServerConfig &other) {
    if (this != &other) {
        _port = other._port;
        _root = other._root;
        _serverNames = other._serverNames;
        _errorPages = other._errorPages;
        _locations = other._locations;
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

const std::string &ServerConfig::getPort() const { return _port; }

const std::string &ServerConfig::getRoot() const { return _root; }

const std::vector<std::string> ServerConfig::getServerNames() const {
    return _serverNames;
}

const std::map<std::string, std::string> ServerConfig::getErrorPages() const {
    return _errorPages;
}

void ServerConfig::setPort(const std::string &port) { _port = port; }

void ServerConfig::setRoot(const std::string &root) { _root = root; }

void ServerConfig::addServer(const std::string &server) {
    _serverNames.push_back(server);
}

void ServerConfig::addErrorPage(const std::string &errorCode,
                                const std::string &path) {
    _errorPages[errorCode] = path;
}

void ServerConfig::addLocation(const Location &location) {
    _locations.push_back(location);
}
