#include "SimpleServer.hpp"

ft::SimpleServer::SimpleServer(std::string ip, std::string port, int bklg) {
	this->socket = new ListeningSocket(ip, port, bklg);
}

ft::ListeningSocket *ft::SimpleServer::GetSocket() {
    return socket;
}

