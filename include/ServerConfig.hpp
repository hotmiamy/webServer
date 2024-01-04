#pragma once

#include <fstream>
#include <string>

class ServerConfig {
   public:
    ServerConfig(const ServerConfig &);
    ServerConfig &operator=(const ServerConfig &);
    ~ServerConfig();

    static ServerConfig fromFile(const std::string &);

   private:
    ServerConfig();

    void parse(std::ifstream &);
};
