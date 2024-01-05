#pragma once

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "Location.hpp"

class ServerConfig {
   public:
    ServerConfig(const ServerConfig &);
    ServerConfig &operator=(const ServerConfig &);
    ~ServerConfig();

    static std::vector<ServerConfig> fromFile(const std::string &);

   private:
    ServerConfig();

    int _port;
    std::vector<std::string> _serverNames;
    std::map<int, std::string> _errorPages;
    std::vector<Location> _locations;
};
