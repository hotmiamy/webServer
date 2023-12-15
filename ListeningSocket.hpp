#ifndef LISTENINGSOCKET_HPP
# define LISTENINGSOCKET_HPP

#include <stdio.h>
#include "BindingSocket.hpp"

namespace ft
{
	class ListeningSocket : BindingSocket
	{
		private:
			int backlog;
			int listening;
		public:
			ListeningSocket(int domain, int service, int protocol, int port, u_long interface, int bklg);
			void start_listening();
	};
}

#endif