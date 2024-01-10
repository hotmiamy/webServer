#include "TestServer.hpp"

ft::TestServer::TestServer() : SimpleServer("127.0.0.1", "8080", 10)
{
	Launch();
}

void ft::TestServer::Accepter() {
    struct sockaddr_in clientaddr = {};
	socklen_t addrLen = sizeof(clientaddr);
    this->New_Socket = accept(GetSocket()->get_SockFd(), (struct sockaddr *)&clientaddr, (socklen_t *)&addrLen);
    memset(this->Buffer, 0, 3000);
    read(this->New_Socket, this->Buffer, 3000);
}

void ft::TestServer::Launch() {
    while (1) {
		std::cout << "Waiting..." << std::endl;
		Accepter();
		Handler();
		Responder();
    }
}

void ft::TestServer::Handler() {
    std::cout << this->Buffer << std::endl;
}

void ft::TestServer::Responder() {
	send(this->New_Socket, "teste", 6, 0);
	close(this->New_Socket);
}
