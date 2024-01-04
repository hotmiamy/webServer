#pragma once

#include "ServerConfig.hpp"

class WebServer {
   public:
    WebServer();
    WebServer(const WebServer &);
    WebServer &operator=(const WebServer &);
    ~WebServer();

    void run(const ServerConfig &);

   private:
};
