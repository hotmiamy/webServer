#pragma once

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "Location.hpp"

class ServerConfig {
   public:
    ServerConfig();
    ServerConfig(const ServerConfig &);
    ServerConfig &operator=(const ServerConfig &);
    ~ServerConfig();

    static std::vector<ServerConfig> fromFile(const std::string &);

    const std::string &getPort() const;
    const std::vector<std::string> getServerNames() const;

    void setPort(const std::string &);
    void addServer(const std::string &);
    void addErrorPage(int, const std::string &);

   private:
    std::string _port;
    std::vector<std::string> _serverNames;
    std::map<int, std::string> _errorPages;
    std::vector<Location> _locations;
};

typedef std::vector<ServerConfig> ConfigVec;
