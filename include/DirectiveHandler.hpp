#pragma once

#include <sys/stat.h>

#include <algorithm>
#include <limits>
#include <map>
#include <sstream>

#include "ServerConfig.hpp"

class DirectiveHandler {
   public:
    typedef void (DirectiveHandler::*DirectiveFunction)(std::istringstream &,
                                                        ServerConfig &);

    DirectiveHandler();
    ~DirectiveHandler();

    void process(const std::string &, std::istringstream &, ServerConfig &);

   private:
    static const std::string ERR_LISTEN;
    static const std::string ERR_SERVER_NAME;
    static const std::string ERR_ERROR_PAGE;
    static const std::string ERR_LOCATION;
    static const std::string ERR_ALLOWED_METHODS;

    std::map<std::string, DirectiveFunction> _directiveMap;

    void _handleListenDirective(std::istringstream &, ServerConfig &);
    void _handleServerNameDirective(std::istringstream &, ServerConfig &);
    void _handleErrorPageDirective(std::istringstream &, ServerConfig &);
    void _handleLocationDirective(std::istringstream &, ServerConfig &);
    void _handleAllowedMethodsDirective(std::istringstream &, Location &);
    void _handleIndexFiles(std::istringstream &, Location &);

    void _resolvePath(std::istringstream &, Location &);
    void _resolveIndexFiles(std::istringstream &, Location &);

    bool _isFileReadable(const std::string &) const;
    bool _isNumeric(const std::string &) const;
    bool _isAllowedHttpMethod(const std::string &) const;
};
