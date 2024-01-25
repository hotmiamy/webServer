#pragma once

#include <netdb.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#include <vector>

#include "ServerConfig.hpp"

class Socket {
   public:
    Socket(const Socket &);
    Socket(const ServerConfig &);
    Socket &operator=(const Socket &);
    ~Socket();

    const static int ConnectionRequests = 10;

    void connect();
    int accept();
	int getSocketFd() const;

   private:
    Socket();

    int _socketFd;
    int _clientFd;
    struct addrinfo *_res;
    std::vector<std::string> _serverNames;
    std::string _port;

    void _setup();
    template <typename ExceptionType>
    void _checkConnectionThrow(int ret, const ExceptionType &);

};

typedef std::vector<Socket> SocketVec;
