#include "SimpleSocket.hpp"

ft::SimpleSocket::SimpleSocket(int domain, int service, int protocol, int port, u_long interface)
{
	// Define address structure
	_address.sin_family = domain;
	_address.sin_port = htons(port);
	_address.sin_addr.s_addr = htonl(interface);
	// Establish socket
	sock = socket(domain, service, protocol);
}

void ft::SimpleSocket::CheckConnection(int connection)
{
	if (connection < 0)
	{
		perror("failed to connect");
		exit(EXIT_FAILURE);
	}
}

struct sockaddr_in ft::SimpleSocket::get_address()
{
	return _address;
}

int ft::SimpleSocket::get_sock()
{
	return sock;
}
