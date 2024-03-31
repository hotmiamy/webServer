#pragma once

#include <sys/stat.h>
#include <unistd.h>

#include <algorithm>
#include <limits>
#include <map>
#include <sstream>
#include <utility>

#include "ServerConfig.hpp"
#include "ServerUtils.hpp"

class DirectiveHandler {
   public:
    typedef void (DirectiveHandler::*DirectiveFunction)(std::istringstream &);

    DirectiveHandler();
    ~DirectiveHandler();

    void process(const std::string &, std::istringstream &);
    ServerConfig getCfg() const;

   private:
    static const std::string ERR_LISTEN;
    static const std::string ERR_SERVER_NAME;
    static const std::string ERR_ERROR_PAGE;
    static const std::string ERR_LOCATION;
    static const std::string ERR_ALLOWED_METHODS;
    static const std::string ERR_ROOT;
    static const std::string ERR_CGI;
    static const std::string ERR_MAX_BODY_SIZE;
    static const std::string ERR_RETURN;
    static const std::string ERR_AUTOINDEX;

    ServerConfig _cfg;
    std::map<std::string, DirectiveFunction> _directiveMap;

    void _handleListenDirective(std::istringstream &);
    void _handleServerNameDirective(std::istringstream &);
    void _handleErrorPageDirective(std::istringstream &);
    void _handleRoot(std::istringstream &);
    void _handleCgiServer(std::istringstream &);
    void _handleCgi(std::istringstream &, Location &);
    void _handleReturn(std::istringstream &, Location &);
    void _handleAutoindex(std::istringstream &, Location &);
    void _handleLocationDirective(std::istringstream &);
    void _handleAllowedMethodsDirective(std::istringstream &, Location &);
    void _handleIndexFile(std::istringstream &, Location &);
    void _handleClientMaxBodySize(std::istringstream &);

    void _resolvePath(std::istringstream &, Location &);
    void _resolveIndexFile(std::istringstream &, Location &);
    void _resolveAllowedMethods(std::istringstream &, Location &);

    bool _isAllowedHttpMethod(const std::string &) const;
};
