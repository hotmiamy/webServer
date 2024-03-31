#include "Socket.hpp"

Socket::Socket()
    : _serverName(), _res(NULL), _port(), _serverFD(-1), _clientFd(-1) {}

Socket::Socket(const Socket &other) { *this = other; }

Socket::Socket(const ServerConfig &cfg)
    : _serverName(cfg.getServerName()),
      _res(NULL),
      _port(cfg.getPort()),
      _serverFD(-1),
      _clientFd(-1) {}

Socket &Socket::operator=(const Socket &other) {
    if (this != &other) {
        _serverFD = other._serverFD;
        _clientFd = other._clientFd;
        _res = other._res;
        _serverName = other._serverName;
        _port = other._port;
    }
    return *this;
}

Socket::~Socket() {}

int Socket::_setup() {
    struct addrinfo info;
    int serverFD;
    bzero(&info, sizeof(struct addrinfo));
    int optVal = 1;

    info.ai_family = AF_INET;
    info.ai_socktype = SOCK_STREAM;

    std::string ip = "127.0.0.1";

    _checkConnectionThrow(
        ::getaddrinfo(ip.c_str(), _port.c_str(), &info, &_res),
        std::runtime_error("'getaddrinfo' failed"));

    serverFD = ::socket(_res->ai_family, _res->ai_socktype, _res->ai_protocol);

    _checkConnectionThrow(
        setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal)),
        std::runtime_error("'setsockopt' failed"));

    _checkConnectionThrow(bind(serverFD, _res->ai_addr, _res->ai_addrlen),
                          std::runtime_error("'bind' failed"));

    freeaddrinfo(_res);

    _checkConnectionThrow(listen(serverFD, Socket::ConnectionRequests),
                          std::runtime_error("'listen' failed"));
    return serverFD;
}

int Socket::connect() { return _setup(); }

int Socket::accept(int serverFD) {
    sockaddr_storage clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    bzero(&clientAddr, clientAddrSize);

    _serverFD = serverFD;
    _clientFd =
        ::accept(serverFD, (struct sockaddr *)&clientAddr, &clientAddrSize);

    if (_clientFd == -1) return _clientFd;
    return _clientFd;
}

int Socket::read(std::string &request) {
	char buff[4096] = {0};
	int bytesread, totalbytesread = 0;

    while ((bytesread = recv(this->_clientFd, buff, sizeof(buff), 0))) {
        if (bytesread >= 0) {
            request.append(buff, bytesread);
            totalbytesread += bytesread;
        }
        if (request.find("POST") != std::string::npos) {
            if (request.find("Transfer-Encoding: chunked") !=
                std::string::npos) {
                if (request.find("\r\n0\r\n") != std::string::npos)
                    break;
                else
                    continue;
            } else if (request.find("Content-Length:") != std::string::npos) {
                std::size_t startPos = request.find("Content-Length: ") + 16;
                std::size_t endPos = request.find("\r\n", startPos);
                std::string requestLength =
                    request.substr(startPos, endPos - startPos);
                std::string body = request.substr(request.find("\r\n\r\n") + 4);
                std::istringstream issRequestLength(requestLength);
                issRequestLength >> _contentLength;

                if (body.size() >= _contentLength) {
                    break;
                } else
                    continue;
            } else {
                break;
            }
        }
        if (request.find("Expect: 100-continue") != std::string::npos)
            sleep(1);
        else if (request.find("\r\n\r\n") != std::string::npos)
            break;
        if (bytesread <= 0) break;
    }
    return ((bytesread <= 0) ? bytesread : totalbytesread);
}

int Socket::send(const std::string &response) {
	long int bytesreturned = 0, totalbytes = 0;
	std::cout << SEPARATOR << response.substr(0, response.find("\r\n\r\n")) << SEPARATOR;
	while ((size_t)totalbytes < response.size())
	{

		bytesreturned = ::send(this->_clientFd, response.c_str(), response.size(), 0);
		totalbytes += bytesreturned;
		if (bytesreturned < 0)
			break;
	}
	return (totalbytes);
}

template <typename ExceptionType>
void Socket::_checkConnectionThrow(int ret, const ExceptionType &exception) {
    if (ret < 0) {
        throw exception;
    }
}

int Socket::getServerFD() const { return _serverFD; }

int Socket::getClientFd() const { return _clientFd; }
