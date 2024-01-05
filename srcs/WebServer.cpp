#include "WebServer.hpp"

#include "ServerConfig.hpp"

WebServer::WebServer() {}

WebServer::WebServer(const ServerConfig &) {}

WebServer::WebServer(const WebServer &other) { *this = other; }

WebServer &WebServer::operator=(const WebServer &other) {
    if (this != &other) {
    }
    return *this;
}

WebServer::~WebServer() {}
