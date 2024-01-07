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
			ConnectingSocket(std::string ip, std::string port);
			void ConnectToNetwork(int SockFd, struct addrinfo *addrRes);
	};
}

#endif