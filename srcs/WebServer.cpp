#include "WebServer.hpp"

WebServer::WebServer() {}

WebServer::WebServer(const ServerVec &server)
    : _keepAlive(), _rawRequest(), _server(server), _timeout(false), _poll() {}

WebServer::WebServer(const WebServer &other) { *this = other; }

WebServer &WebServer::operator=(const WebServer &other) {
    if (this != &other) {
        _keepAlive = other._keepAlive;
        _rawRequest = other._rawRequest;
        _server = other._server;
        _timeout = other._timeout;
        _poll = other._poll;
    }
    return *this;
}

WebServer::~WebServer() { _poll.clearAllFds(); }

void WebServer::init() {
    for (ServerVec::size_type i = 0; i < _server.size(); ++i) {
        Socket socket(_server.at(i));
        int serverFD = socket.connect();
        if (fcntl(serverFD, F_SETFL, O_NONBLOCK) < 0)
            std::__throw_runtime_error("Error set nonblocking I/O");
        this->_poll.addFd(socket, serverFD);
        _server[i].setSocketFD(serverFD);
    }
    launch();
}

void WebServer::launch() {
    while (true) {
        try {
            this->_poll.execute();
            for (size_t i = 0; i < this->_poll.getSize(); ++i) {
                std::time_t now;
                std::time(&now);
                long diff = 0;

                if (i >= _server.size()) {
                    diff = std::difftime(
                        now,
                        _keepAlive[this->_poll.getSocket(i).getClientFd()]);
                }
                if (this->_poll.checkEvent(i, diff, _timeout)) {
                    Socket event = this->_poll.getSocket(i);
                    struct pollfd pollEvent = this->_poll.getEvent()[i];
                    if (pollEvent.revents & POLLHUP) {
                        std::cout << "Client on fd: " << event.getClientFd()
                                  << " disconnected hang up" << std::endl;
                        _keepAlive.erase(event.getClientFd());
                        _poll.removeEventFd(event);
                    } else if (pollEvent.revents & POLLERR) {
                        std::cout << "Client on fd: " << event.getClientFd()
                                  << " disconnected error" << std::endl;
                        _keepAlive.erase(event.getClientFd());
                        _poll.removeEventFd(event);
                    } else if (i < _server.size()) {
                        if (event.accept(_server[i].getSocketFD()) < 0)
                            continue;
                        std::cout
                            << "New connection on fd: " << event.getClientFd()
                            << std::endl;
                        _keepAlive[event.getClientFd()] = now;
                        if (fcntl(event.getClientFd(), F_SETFL, O_NONBLOCK) < 0)
                            std::__throw_runtime_error(
                                "Error set nonblocking I/O");
                        _poll.addFd(event, event.getClientFd());
                    } else {
                        int clientRes = event.read(_rawRequest);
                        if (clientRes > 0 || _timeout == true) {
                            respond(event, clientRes);
                        }
                    }
                }
            }
        } catch (const std::exception &e) {
            std::cerr << e.what() << '\n';
        }
    }
}

void WebServer::respond(Socket &client, int clientRes) {
    std::time_t now;
    std::time(&now);
    ReqParsing req(getCurrentServer(client));

    if (_timeout == true) {
        _timeout = false;
        std::cout << "Client on fd: " << client.getClientFd() << " timeout"
                  << std::endl;
        _keepAlive.erase(client.getClientFd());
        _poll.removeEventFd(client);
        return;
    }
    std::cout << "Request of FD: " << client.getClientFd();
    std::cout << SEPARATOR
              << _rawRequest.substr(0, _rawRequest.find("\r\n\r\n"))
              << SEPARATOR;
    req.parse(_rawRequest, clientRes);
    if ((req.getIsParsed() == true || req.getStatusCode().empty() == false)) {
        Response res(req);
        int sendRes;

        std::cout << "Response to connection on Fd " << client.getClientFd();
        sendRes = client.send(res._response);
        if (res.getStatusCode() >= 400 || sendRes < 0) {
            _poll.removeEventFd(client);
            _keepAlive.erase(client.getClientFd());
            std::cout << "Client on Fd: " << client.getClientFd();
            if (sendRes < 0)
                std::cout << " send error " << std::endl;
            else
                std::cout << " removed" << std::endl;
        }
        _rawRequest = "";
    }
}

const ServerConfig &WebServer::getCurrentServer(const Socket &socket) {
    for (size_t i = 0; i < _server.size(); ++i) {
        if (_server[i].getSocketFD() == socket.getServerFD()) {
            return _server[i];
        }
    }
    throw std::runtime_error("Socket not found");
}

void WebServer::stop() { _poll.clearAllFds(); }
