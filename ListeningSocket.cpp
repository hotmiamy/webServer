#include "ListeningSocket.hpp"

ft::ListeningSocket::ListeningSocket(std::string ip, std::string port, int bklg) : BindingSocket(ip, port)
{
	backlog = bklg;
	start_listening();
	CheckConnection(listening);
}

void ft::ListeningSocket::start_listening()
{
	listening = listen(get_SockFd(), backlog); 
}