#pragma once

#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <vector>

#include "Socket.hpp"

class WebServer {
   public:
    WebServer();
    WebServer(const WebServer &);
    WebServer &operator=(const WebServer &);
    ~WebServer();

    void run(const ConfigVec &configs);

   private:
    char _buff[3000];
    int _newSock;

    void _launch(SocketVec &);
    void _read();
    void _respond();
};
