#include "TestServer.hpp"

ft::TestServer::TestServer() : SimpleServer(AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY, 10)
{
    Launch();
}

void ft::TestServer::Accepter() {
    struct sockaddr_in address = GetSocket()->get_address();
    int addrLen = sizeof(address);
    this->New_Socket = accept(GetSocket()->get_sock(), (struct sockaddr *)&address, (socklen_t *)&addrLen);
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
    write(this->New_Socket, "teste", 6);
    close(this->New_Socket);
}

