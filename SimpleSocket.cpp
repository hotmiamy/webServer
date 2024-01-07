#include "SimpleSocket.hpp"

ft::SimpleSocket::SimpleSocket(std::string ip, std::string port)
{
	// Define address structure
	struct addrinfo info = {};

	info.ai_family = AF_UNSPEC;
	info.ai_socktype = SOCK_STREAM;
	// Establish socket
	CheckConnection(getaddrinfo(ip.c_str(), port.c_str(), &info, &res));
	_SockFd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
}

void ft::SimpleSocket::CheckConnection(int connection)
{
	if (connection < 0)
	{
		perror("failed to connect");
		exit(EXIT_FAILURE);
	}
}

struct addrinfo *ft::SimpleSocket::get_AddrRes()
{
	return res;
}

int ft::SimpleSocket::get_SockFd()
{
	return _SockFd;
}
