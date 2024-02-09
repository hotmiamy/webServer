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
    const std::vector<std::string> getServerNames() const;
    const std::map<std::string, std::string> getErrorPages() const;
	const std::map<std::string, Location> &getLocations() const;

    void setPort(const std::string &);
    void setRoot(const std::string &);
    void addServer(const std::string &);
    void addErrorPage(const std::string &, const std::string &);
    void addLocation(std::string path, const Location &);

    bool good() const;

   private:
	// Port to listen on
    std::string _port;
	//  root directory of the server
    std::string _root;
	//	List of servers names (ex: localhost)
    std::vector<std::string> _serverNames;
	//  Error pages for HTTP errors
    std::map<std::string, std::string> _errorPages;
	// class of locations
    std::map<std::string, Location> _locations;
};

typedef std::vector<ServerConfig> ConfigVec;
