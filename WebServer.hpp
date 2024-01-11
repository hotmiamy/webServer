#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

#include "Socket.hpp"
#include <unistd.h>
#include <iostream>
#include <cstring>

namespace ft {
    class WebServer {
        private:
            void Read();
            void Responder();
            int New_Socket;
			char Buffer[3000];
        
        public:
            WebServer();
            void Launch(Socket *socket);
            
    };
}

#endif