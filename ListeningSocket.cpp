#include "ListeningSocket.hpp"

ft::ListeningSocket::ListeningSocket(int domain, int service, int protocol, int port, u_long interface, int bklg) : BindingSocket(domain, service, protocol, port, interface)
{
	backlog = bklg;
	start_listening();
	CheckConnection(listening);
}

void ft::ListeningSocket::start_listening()
{
	listening = listen(get_sock(), backlog); 
}