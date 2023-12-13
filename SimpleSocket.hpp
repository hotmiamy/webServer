#ifndef SIMPLESOCKET_HPP
# define SIMPLESOCKET_HPP

#include <cstdio>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>

namespace ft
{
	class SimpleSocket
	{
		private:
			struct sockaddr_in	_address;
			int sock;
			int connection;
		public:
			SimpleSocket(int domain, int service, int protocol, int port, u_long interface);
			virtual int	ConnectToNetwork(int sock, struct sockaddr_in address);
			void 		CheckConnection(int);
	};
}

#endif