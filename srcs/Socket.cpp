#include "Socket.hpp"

Socket::Socket()
    : _socketFd(-1), _clientFd(-1), _res(NULL), _serverName(), _port() {}

Socket::Socket(const Socket &other) { *this = other; }

Socket::Socket(const ServerConfig &cfg)
    : _socketFd(-1),
      _clientFd(-1),
      _res(NULL),
      _serverName(cfg.getServerName()),
      _port(cfg.getPort()) {}

Socket &Socket::operator=(const Socket &other) {
    if (this != &other) {
        _socketFd = other._socketFd;
        _clientFd = other._clientFd;
        _res = other._res;
        _serverName = other._serverName;
        _port = other._port;
    }
    return *this;
}

Socket::~Socket() {}

void Socket::_setup() {
    struct addrinfo info;
    bzero(&info, sizeof(struct addrinfo));

    int optVal = 1;
    info.ai_family = AF_UNSPEC;
    info.ai_socktype = SOCK_STREAM;

    // TODO: handle multiple IPs/hosts (probably not the best way to do it...)
    std::string ip = "127.0.0.1";

    _checkConnectionThrow(
        ::getaddrinfo(ip.c_str(), _port.c_str(), &info, &_res),
        std::runtime_error("..."));

    _socketFd = ::socket(_res->ai_family, _res->ai_socktype, _res->ai_protocol);

    _checkConnectionThrow(setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR,
                                     &optVal, sizeof(optVal)),
                          std::runtime_error("..."));
}

void Socket::connect() {
    _setup();
    _checkConnectionThrow(bind(_socketFd, _res->ai_addr, _res->ai_addrlen),
                          std::runtime_error("..."));
    _checkConnectionThrow(listen(_socketFd, Socket::ConnectionRequests),
                          std::runtime_error("..."));
}

int Socket::accept() {
    sockaddr_storage clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    bzero(&clientAddr, clientAddrSize);

    _clientFd =
        ::accept(_socketFd, (struct sockaddr *)&clientAddr, &clientAddrSize);

    _checkConnectionThrow(_clientFd, std::runtime_error("..."));
    return _clientFd;
}

template <typename ExceptionType>
void Socket::_checkConnectionThrow(int ret, const ExceptionType &exception) {
    if (ret < 0) {
        throw exception;
    }
}

int Socket::getSocketFd() const { return _socketFd; }
