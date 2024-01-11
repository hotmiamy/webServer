#pragma once

#include <netdb.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>

#include <vector>

#include "ServerConfig.hpp"

class Socket {
   public:
    typedef std::vector<Socket> SocketVec;

    Socket(const Socket &);
    Socket(const ServerConfig &);
    Socket &operator=(const Socket &);
    ~Socket();

    const static int ConnectionRequests = 10;

    void connect();

   private:
    Socket();

    int _socketFd;
    int _clientFd;
    struct addrinfo *_res;
    std::vector<std::string> _serverNames;
    std::string _port;

    void _setup();
    template <typename ExceptionType>
    void _checkConnectionOrElseThrow(int ret, const ExceptionType &);
};
