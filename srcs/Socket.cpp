#include "Socket.hpp"

Socket::Socket() : _socketFd(-1), _clientFd(-1), _res() {}

Socket::Socket(const Socket &other) { *this = other; }

Socket::Socket(const ServerConfig &cfg)
    : _socketFd(-1),
      _clientFd(-1),
      _res(),
      _serverNames(cfg.getServerNames()),
      _port(cfg.getPort()) {}

Socket &Socket::operator=(const Socket &other) {
    if (this != &other) {
        _socketFd = other._socketFd;
        _clientFd = other._clientFd;
        _res = other._res;
    }
    return *this;
}

Socket::~Socket() {}

void Socket::connect() { this->_setup(); }

void Socket::_setup() {
    struct addrinfo info;
    bzero(&info, sizeof(struct addrinfo));

    int optVal = 1;
    info.ai_family = AF_UNSPEC;
    info.ai_socktype = SOCK_STREAM;

    // TODO: handle multiple IPs/hosts (probably not the best way to do it...)
    std::string ip = _serverNames.at(0);
    if (::getaddrinfo(ip.c_str(), _port.c_str(), &info, &_res) < 0) {
        throw std::runtime_error("");
    }

    _socketFd = ::socket(_res->ai_family, _res->ai_socktype, _res->ai_protocol);

    if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &optVal,
                   sizeof(optVal)) < 1) {
        throw std::runtime_error("");
    }
}
