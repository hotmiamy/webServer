#ifndef SIMPLESERVER_HPP
# define SIMPLESERVER_HPP

#include "master.hpp"

namespace ft
{
	class SimpleServer
	{
		private:
			ListeningSocket* socket;
            virtual void Accepter() = 0;
            virtual void Handler() = 0;
            virtual void Responder() = 0;

		public:
            SimpleServer(int domain, int service, int protocol, int port, u_long interface, int bklg);
            ListeningSocket *GetSocket();
            virtual void Launch() = 0;
    };
}

#endif