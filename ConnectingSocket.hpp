#ifndef CONNECTING_SOCKET_HPP
# define CONNECTING_SOCKET_HPP

#include <stdio.h>

#include "SimpleSocket.hpp"

namespace ft
{
	class ConnectingSocket : public SimpleSocket
	{
		private:
			int connection;
		public:
			ConnectingSocket(int domain, int service, int protocol, int port, u_long interface);
			void ConnectToNetwork(int sock, struct sockaddr_in address);
	};
}

#endif