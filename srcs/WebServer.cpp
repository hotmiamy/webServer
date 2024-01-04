#include "WebServer.hpp"

#include "ServerConfig.hpp"

WebServer::WebServer() {}

WebServer::WebServer(const WebServer &other) { *this = other; }

WebServer &WebServer::operator=(const WebServer &other) {
    if (this != &other) {
    }
    return *this;
}

WebServer::~WebServer() {}

void WebServer::run(const ServerConfig &config) {
    for (;;) {
    }
}
