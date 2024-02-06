#pragma once

#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#include <cstring>
#include <iostream>
#include <vector>

#include "Socket.hpp"
#include "Poll.hpp"
#include "Client.hpp"

class WebServer {
   public:
    WebServer();
    WebServer(const WebServer &);
    WebServer &operator=(const WebServer &);
    ~WebServer();

    void	run(const ConfigVec &configs);

   private:
	Poll	_poll;
	int		_newSock;

    void	_launch(SocketVec &);
    void	_read();
    void	_respond(std::string response);
};
