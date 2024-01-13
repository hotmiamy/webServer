#pragma once

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
    std::map<std::string, DirectiveFunction> _directiveMap;

    void _handleListenDirective(std::istringstream &, ServerConfig &);
    void _handleServerNameDirective(std::istringstream &, ServerConfig &);
    void _handleErrorPageDirective(std::istringstream &, ServerConfig &);
    void _handleLocationDirective(std::istringstream &, ServerConfig &);
    void _handleAllowedMethodsDirective(std::istringstream &, Location &);
};
