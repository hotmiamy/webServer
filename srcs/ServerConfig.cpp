#include "ServerConfig.hpp"

#include "ConfigParser.hpp"

ServerConfig::ServerConfig()
    : _port(),
      _root(),
      _serverName(),
      _errorPages(),
      _locations(),
      _clientMaxBodySize(),
	  _socketFd(){}

ServerConfig::ServerConfig(const ServerConfig &other) { *this = other; }

ServerConfig &ServerConfig::operator=(const ServerConfig &other) {
    if (this != &other) {
        _port = other._port;
        _root = other._root;
        _serverName = other._serverName;
        _errorPages = other._errorPages;
        _locations = other._locations;
        _clientMaxBodySize = other._clientMaxBodySize;
		_socketFd = other._socketFd;
    }
    return *this;
}

ServerConfig::~ServerConfig() {}

std::vector<ServerConfig> ServerConfig::fromFile(const std::string &file) {
    std::ifstream ifs(file.c_str());

    if (!ifs.is_open()) {
        throw std::invalid_argument("invalid file: " + file);
    }

    return ConfigParser::parse(ifs);
}

const std::string &ServerConfig::getPort() const { return _port; }

const std::string &ServerConfig::getRoot() const { return _root; }

const std::string &ServerConfig::getServerName() const { return _serverName; }

const int &ServerConfig::getSocketFD() const { return _socketFd; }

const std::map<std::string, std::string> &ServerConfig::getErrorPages() const {
    return _errorPages;
}

const std::map<std::string, Location> &ServerConfig::getLocations() const {
    return _locations;
}

const std::string &ServerConfig::getClientMaxBodySize() const {
    return _clientMaxBodySize;
}

void ServerConfig::setPort(const std::string &port) { _port = port; }

void ServerConfig::setRoot(const std::string &root) { _root = root; }

void ServerConfig::setServer(const std::string &server) { _serverName = server; }

void ServerConfig::setSocketFD(const int &socket) { _socketFd = socket; }

void ServerConfig::addErrorPage(const std::string &errorCode,
                                const std::string &path) {
    _errorPages[errorCode] = path;
}

void ServerConfig::addLocation(std::string path, const Location &location) {
    _locations.insert(make_pair(path, location));
}

void ServerConfig::setClientMaxBodySize(const std::string &clientMaxBodySize) {
    _clientMaxBodySize = clientMaxBodySize;
}

bool ServerConfig::good() const {
    return !_port.empty() && !_root.empty() && !_serverName.empty() &&
           !_errorPages.empty() && !_locations.empty();
}
