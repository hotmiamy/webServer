#include "DirectiveHandler.hpp"

#include <iostream>

DirectiveHandler::DirectiveHandler() {
    _directiveMap["listen"] = &DirectiveHandler::_handleListenDirective;
    _directiveMap["server_name"] =
        &DirectiveHandler::_handleServerNameDirective;
    _directiveMap["error_page"] = &DirectiveHandler::_handleErrorPageDirective;
}

DirectiveHandler::~DirectiveHandler() {}

void DirectiveHandler::_handleListenDirective(std::istringstream &iss,
                                              ServerConfig &cfg) {
    std::string port;
    iss >> port;
    cfg.setPort(port);
}

void DirectiveHandler::_handleServerNameDirective(std::istringstream &iss,
                                                  ServerConfig &cfg) {
    std::string server;
    while (iss >> server) {
        cfg.addServer(server);
    }
}

void DirectiveHandler::_handleErrorPageDirective(std::istringstream &iss,
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
