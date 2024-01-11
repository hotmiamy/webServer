#include "DirectiveHandler.hpp"

#include <iostream>

DirectiveHandler::DirectiveHandler() {
    _directiveMap["listen"] = &DirectiveHandler::handleListenDirective;
    _directiveMap["server_name"] = &DirectiveHandler::handleServerNameDirective;
    _directiveMap["error_page"] = &DirectiveHandler::handleErrorPageDirective;
}

DirectiveHandler::~DirectiveHandler() {}

void DirectiveHandler::handleListenDirective(std::istringstream &iss,
                                             ServerConfig &cfg) {
    std::string port;
    iss >> port;
    cfg.setPort(port);
}

void DirectiveHandler::handleServerNameDirective(std::istringstream &iss,
                                                 ServerConfig &cfg) {
    std::string server;
    while (iss >> server) {
        cfg.addServer(server);
    }
}

void DirectiveHandler::handleErrorPageDirective(std::istringstream &iss,
                                                ServerConfig &cfg) {
    int errorCode;
    std::string path;
    iss >> errorCode >> path;
    cfg.addErrorPage(errorCode, path);
}

void DirectiveHandler::process(const std::string &directive,
                               std::istringstream &iss, ServerConfig &cfg) {
    if (_directiveMap.find(directive) != _directiveMap.end()) {
        (this->*_directiveMap[directive])(iss, cfg);
    }
}
