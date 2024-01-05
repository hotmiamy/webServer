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

    void handleListenDirective(std::istringstream &, ServerConfig &);
    void handleServerNameDirective(std::istringstream &, ServerConfig &);
    void handleErrorPageDirective(std::istringstream &, ServerConfig &);
    void process(const std::string &, std::istringstream &, ServerConfig &);

   private:
    std::map<std::string, DirectiveFunction> _directiveMap;
};
