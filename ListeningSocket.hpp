#ifndef LISTENINGSOCKET_HPP
# define LISTENINGSOCKET_HPP

#include <stdio.h>
#include "BindingSocket.hpp"

namespace ft
{
	class ListeningSocket : public BindingSocket
	{
		private:
			int backlog;
			int listening;
		public:
			ListeningSocket(std::string ip, std::string port, int bklg);
			void start_listening();
	};
}

#endif