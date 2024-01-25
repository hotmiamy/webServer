#pragma once

#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#include <cstring>
#include <iostream>
#include <vector>

#include "Socket.hpp"
#include "Poll.hpp"

class WebServer {
   public:
    WebServer();
    WebServer(const WebServer &);
    WebServer &operator=(const WebServer &);
    ~WebServer();

    void run(const ConfigVec &configs);

   private:
	Poll _poll;
	char _buff[3000];
	int _newSock;

    void _launch(SocketVec &);
    void _read();
    void _respond();
};
