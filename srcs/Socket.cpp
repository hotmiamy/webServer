#include "Socket.hpp"

Socket::Socket()
    : _socketFd(-1), _clientFd(-1), _res(NULL), _serverNames(), _port() {}

Socket::Socket(const Socket &other) { *this = other; }

Socket::Socket(const ServerConfig &cfg)
    : _socketFd(-1),
      _clientFd(-1),
      _res(NULL),
      _serverNames(cfg.getServerNames()),
      _port(cfg.getPort()) {}

Socket &Socket::operator=(const Socket &other) {
    if (this != &other) {
        _socketFd = other._socketFd;
        _clientFd = other._clientFd;
        _res = other._res;
        _serverNames = other._serverNames;
        _port = other._port;
    }
    return *this;
}

Socket::~Socket() {
    // if (_res) {
    //     freeaddrinfo(_res);
    // }
    // if (_socketFd != -1) {
    //     close(_socketFd);
    // }
}

void Socket::connect() {
    this->_setup();

    _checkConnectionOrElseThrow(
        bind(_socketFd, _res->ai_addr, _res->ai_addrlen),
        std::runtime_error("..."));

    _checkConnectionOrElseThrow(listen(_socketFd, Socket::ConnectionRequests),
                                std::runtime_error("..."));
}

int Socket::accept() {
    sockaddr_storage clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    bzero(&clientAddr, clientAddrSize);

    _clientFd =
        ::accept(_socketFd, (struct sockaddr *)&clientAddr, &clientAddrSize);

    _checkConnectionOrElseThrow(_clientFd, std::runtime_error("..."));
    return _clientFd;
}

void Socket::_setup() {
    struct addrinfo info;
    bzero(&info, sizeof(struct addrinfo));

    int optVal = 1;
    info.ai_family = AF_UNSPEC;
    info.ai_socktype = SOCK_STREAM;

    // TODO: handle multiple IPs/hosts (probably not the best way to do it...)
    std::string ip = _serverNames.at(0);

    _checkConnectionOrElseThrow(
        ::getaddrinfo(ip.c_str(), _port.c_str(), &info, &_res),
        std::runtime_error("..."));

    _socketFd = ::socket(_res->ai_family, _res->ai_socktype, _res->ai_protocol);

    _checkConnectionOrElseThrow(setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR,
                                           &optVal, sizeof(optVal)),
                                std::runtime_error("..."));
}

template <typename ExceptionType>
void Socket::_checkConnectionOrElseThrow(int ret,
                                         const ExceptionType &exception) {
    if (ret < 0) {
        throw exception;
    }
}
