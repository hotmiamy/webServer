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

#define SEPARATOR "\n-------------------------------------------------\n"

class WebServer {
   public:
	WebServer();
    WebServer(ServerVec server);
    WebServer(const WebServer &);
    WebServer &operator=(const WebServer &);
    ~WebServer();

    void	init();
	void	stop();

   private:
	std::map<int, time_t> _keepAlive;
	std::string _rawRequest;
	ServerVec _server;
	bool	_timeout;
	Poll	_poll;


    void	launch();
    int		read(Socket &socket);
    void	respond(Socket &client, int clientRes);

	const ServerConfig &getCurrentServer(const Socket &socket);
};
