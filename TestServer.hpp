#ifndef TESTSERVER_HPP
# define TESTSERVER_HPP

#include "SimpleServer.hpp"
#include <unistd.h>
#include <iostream>
#include <cstring>

namespace ft {
    class TestServer : public SimpleServer {
        private:
            void Accepter();
            void Handler();
            void Responder();
            int New_Socket;
			char Buffer[3000];
        
        public:
            TestServer();
            void Launch();
            
    };
}

#endif