#ifndef BINDING_SOCKET_HPP
# define BINDING_SOCKET_HPP

#include <stdio.h>
#include "SimpleSocket.hpp"

namespace ft
{
	class BindingSocket: public SimpleSocket
	{
		private:
			int binding;
		public:
			BindingSocket(std::string ip, std::string port);
			void ConnectToNetwork(int SockFd, struct addrinfo *AddrRes);
	};
}


#endif