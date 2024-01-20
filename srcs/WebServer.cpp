#include "WebServer.hpp"

WebServer::WebServer() : _buff(), _newSock(0) {}

WebServer::WebServer(const WebServer &other) { *this = other; }

WebServer &WebServer::operator=(const WebServer &other) {
    if (this != &other) {
        std::strcpy(_buff, other._buff);
        _newSock = other._newSock;
    }
    return *this;
}

WebServer::~WebServer() {}

void WebServer::run(const ConfigVec &configs) {
    SocketVec socketVec;

    for (ConfigVec::size_type i = 0; i < configs.size(); ++i) {
        Socket socket(configs.at(i));
        socket.connect();
        socketVec.push_back(socket);
    }
    _launch(socketVec);
}

void WebServer::_launch(SocketVec &socketVec) {
    for (SocketVec::size_type i = 0; i < socketVec.size(); ++i) {
        for (;;) {
            _newSock = socketVec[i].accept();
            _read();
            _respond();
        }
    }
}

void WebServer::_read() {
    bzero(_buff, sizeof(_buff));
    recv(_newSock, _buff, sizeof(_buff), 0);
    std::cout << _buff << '\n';
}

void WebServer::_respond() {
    send(_newSock, "teste", 6, 0);
    close(_newSock);
}
