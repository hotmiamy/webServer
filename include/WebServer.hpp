#pragma once

#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>

#include "Poll.hpp"
#include "ReqParsing.hpp"
#include "Response.hpp"
#include "Socket.hpp"

#define SEPARATOR "\n-------------------------------------------------\n"

class WebServer {
   public:
    WebServer();
    WebServer(const ServerVec &);
    WebServer(const WebServer &);
    WebServer &operator=(const WebServer &);
    ~WebServer();

    void init();

   private:
    std::map<int, time_t> _keepAlive;
    std::string _rawRequest;
    ServerVec _server;
    bool _timeout;
    Poll _poll;

    void _launch();
    int _read(Socket &socket);
    void _respond(Socket &client, int clientRes);

    const ServerConfig &getCurrentServer(const Socket &socket);
};
