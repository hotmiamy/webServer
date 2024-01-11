#include "WebServer.hpp"

WebServer::WebServer() {}

WebServer::WebServer(const ServerConfig &) {}

WebServer::WebServer(const WebServer &other) { *this = other; }

WebServer &WebServer::operator=(const WebServer &other) {
    if (this != &other) {
    }
    return *this;
}

WebServer::~WebServer() {}

void WebServer::run(const ConfigVec &configs) {
    for (ConfigVec::size_type i = 0; i < configs.size(); ++i) {
        Socket socket(configs.at(i));
        socket.connect();
    }
}
