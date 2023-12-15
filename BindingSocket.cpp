#include "BindingSocket.hpp"

ft::BindingSocket::BindingSocket(int domain, int service, int protocol, int port, u_long interface) : SimpleSocket(domain, service, protocol, port, interface)
{
	ConnectToNetwork(get_sock(), get_address());
	CheckConnection(binding);
}

void ft::BindingSocket::ConnectToNetwork(int sock, struct sockaddr_in address)
{
	binding = bind(sock, (struct sockaddr *)&address, sizeof(address));
}