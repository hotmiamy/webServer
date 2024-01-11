#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <cstdio> //perror
#include <iostream> //cerr
#include <cstdlib> //exit
#include <sys/socket.h> //socket
#include <string.h> //strerror
#include <errno.h> //errno
#include <unistd.h> //close
#include <netdb.h> //socket, bind, addrinfo

namespace ft
{
	class Socket
	{
		private:
			struct addrinfo *res;
			std::string ip;
			std::string port;
			int _SockFd;
			int _ClientFd;

		public:
			Socket(std::string ip, std::string port);
			~Socket();
			void socket();
			void bind();
			void listen(int ConnectionQueue);
			void connect();
			int accept();
			void 			CheckConnection(int);

			struct addrinfo *get_AddrRes();
		
			int get_SockFd();

	};
}

#endif