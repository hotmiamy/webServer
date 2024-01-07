#include "ConnectingSocket.hpp"

ft::ConnectingSocket::ConnectingSocket(std::string ip, std::string port) : SimpleSocket(ip, port)
{
	ConnectToNetwork(get_SockFd(), get_AddrRes());
	CheckConnection(connection);
}

void ft::ConnectingSocket::ConnectToNetwork(int sock, struct addrinfo *AddrRes)
{
	connection = connect(sock, AddrRes->ai_addr, AddrRes->ai_addrlen);
}