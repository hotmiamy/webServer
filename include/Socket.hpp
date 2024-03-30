#pragma once

#include <netdb.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <vector>

#include "ServerConfig.hpp"

#define SEPARATOR "\n-------------------------------------------------\n"

class Socket {
   public:
	Socket();
    Socket(const Socket &);
    Socket(const ServerConfig &);
    Socket &operator=(const Socket &);
    ~Socket();

    const static int ConnectionRequests = 10;

    int connect();
    void accept(int serverFd);
	int read(std::string &request);
	void send(const std::string &response);


    int getServerFD() const;
	int getClientFd() const;

   private:
    std::string _serverName;
    struct addrinfo *_res;
    std::string _port;
    int _serverFD;
    int _clientFd;
	std::size_t _contentLength;

    int _setup();
    template <typename ExceptionType>
    void _checkConnectionThrow(int ret, const ExceptionType &);
};
