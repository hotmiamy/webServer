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
    const std::string &getServerName() const;
    const std::map<std::string, std::string> &getErrorPages() const;
    const std::map<std::string, Location> &getLocations() const;
    const std::string &getClientMaxBodySize() const;
	const int &getSocketFD() const;

    void setPort(const std::string &);
    void setRoot(const std::string &);
    void setServer(const std::string &);
	void setSocketFD(const int &);
    void addErrorPage(const std::string &, const std::string &);
    void addLocation(std::string path, const Location &);
    void setClientMaxBodySize(const std::string &);

    bool good() const;

   private:
    std::string _port;
    std::string _root;
    std::string _serverName;
    std::map<std::string, std::string> _errorPages;
    std::map<std::string, Location> _locations;
    std::string _clientMaxBodySize;
	int _socketFd;
};

typedef std::vector<ServerConfig> ServerVec;
