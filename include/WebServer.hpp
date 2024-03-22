#pragma once

#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>

#include "Socket.hpp"
#include "Poll.hpp"
#include "ReqParsing.hpp"
#include "Response.hpp"

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

    void	_launch(SocketVec &socketVec, const ConfigVec &conf);
    void	_read(const ServerConfig &conf);
    void	_respond(std::string response);
};
