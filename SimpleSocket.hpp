#ifndef SIMPLESOCKET_HPP
# define SIMPLESOCKET_HPP

#include <cstdio> //perror
#include <string> //string
#include <cstdlib> //exit
#include <sys/socket.h> //socket
#include <sys/types.h>
#include <netdb.h>

namespace ft
{
	class SimpleSocket
	{
		private:
			struct addrinfo *res;
			std::string ip;
			std::string port;
			int _SockFd;
		public:
			SimpleSocket(std::string ip, std::string port);
			virtual void	ConnectToNetwork(int sock, struct addrinfo *address) = 0;
			void 			CheckConnection(int);
			struct addrinfo *get_AddrRes();
		
		int get_SockFd();

	};
}

#endif