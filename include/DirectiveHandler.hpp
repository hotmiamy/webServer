#pragma once

#include <sys/stat.h>
#include <unistd.h>

#include <algorithm>
#include <limits>
#include <map>
#include <sstream>

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

    ServerConfig _cfg;
    std::map<std::string, DirectiveFunction> _directiveMap;

    void _handleListenDirective(std::istringstream &);
    void _handleServerNameDirective(std::istringstream &);
    void _handleErrorPageDirective(std::istringstream &);
    void _handleRoot(std::istringstream &);
    void _handleCgi(std::istringstream &);
    void _handleLocationDirective(std::istringstream &);
    void _handleAllowedMethodsDirective(std::istringstream &, Location &);
    void _handleIndexFiles(std::istringstream &, Location &);

    void _resolvePath(std::istringstream &, Location &);
    void _resolveIndexFiles(std::istringstream &, Location &);
    void _resolveAllowedMethods(std::istringstream &, Location &);

    bool _isAllowedHttpMethod(const std::string &) const;
};
