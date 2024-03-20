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
    const std::string &getRoot() const;
    const std::vector<std::string> &getServerNames() const;
    const std::map<std::string, std::string> &getErrorPages() const;
    const std::map<std::string, Location> &getLocations() const;
    const std::string &getClientMaxBodySize() const;
    bool hasCgi() const;

    void setCgi(bool);
    void setPort(const std::string &);
    void setRoot(const std::string &);
    void addServer(const std::string &);
    void addErrorPage(const std::string &, const std::string &);
    void addLocation(std::string path, const Location &);
    void setClientMaxBodySize(const std::string &);

    bool good() const;

   private:
    std::string _port;
    std::string _root;
    std::vector<std::string> _serverNames;
    std::map<std::string, std::string> _errorPages;
    std::map<std::string, Location> _locations;
    bool _cgi;
    std::string _clientMaxBodySize;
};

typedef std::vector<ServerConfig> ConfigVec;
