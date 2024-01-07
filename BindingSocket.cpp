#include "BindingSocket.hpp"

ft::BindingSocket::BindingSocket(std::string ip, std::string port) : SimpleSocket(ip, port)
{
	ConnectToNetwork(get_SockFd(), get_AddrRes());
	CheckConnection(binding);
}

void ft::BindingSocket::ConnectToNetwork(int sock, struct addrinfo *AddrRes)
{
	binding = bind(sock, AddrRes->ai_addr, AddrRes->ai_addrlen);
}