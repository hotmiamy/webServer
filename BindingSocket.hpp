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
			BindingSocket(int domain, int service, int protocol, int port, u_long interface);
			void ConnectToNetwork(int sock, struct sockaddr_in address);
	};
}


#endif