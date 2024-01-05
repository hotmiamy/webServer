#pragma once

#include "ServerConfig.hpp"

class WebServer {
   public:
    WebServer();
    WebServer(const ServerConfig &);
    WebServer(const WebServer &);
    WebServer &operator=(const WebServer &);
    ~WebServer();

   private:
};
