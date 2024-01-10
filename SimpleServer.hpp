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
            SimpleServer(std::string ip, std::string port, int bklg);
            ListeningSocket *GetSocket();
            virtual void Launch() = 0;
    };
}

#endif