#include "ConnectingSocket.hpp"

ft::ConnectingSocket::ConnectingSocket(int domain, int service, int protocol, int port, u_long interface) : SimpleSocket(domain, service, protocol, port, interface)
{
	ConnectToNetwork(get_sock(), get_address());
	CheckConnection(connection);
}

void ft::ConnectingSocket::ConnectToNetwork(int sock, struct sockaddr_in address)
{
	connection = connect(sock, (struct sockaddr *)&address, sizeof(address));
}