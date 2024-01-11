#include "WebServer.hpp"

ft::WebServer::WebServer()
{
	Socket *newsocket = new Socket("127.0.0.1", "8080");
	newsocket->connect();
	Launch(newsocket);
}



void ft::WebServer::Read() 
{
    memset(this->Buffer, 0, 3000);
    recv(this->New_Socket, this->Buffer, 3000, 0);
	std::cout << this->Buffer << std::endl;
}

void ft::WebServer::Launch(Socket *socket) 
{
    while (1) {
		std::cout << "Waiting..." << std::endl;
		New_Socket = socket->accept();
		Read();
		Responder();
	}
}

void ft::WebServer::Responder() 
{
	send(this->New_Socket, "teste", 6, 0);
	close(this->New_Socket);
}
