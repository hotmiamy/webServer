#include "ListeningSocket.hpp"
#include <iostream>

int main()
{
	std::cout << "Starting server..." << std::endl;
	ft::BindingSocket bs = ft::BindingSocket(AF_INET, SOCK_STREAM, 0, 8081, INADDR_ANY);
	ft::ListeningSocket ls = ft::ListeningSocket(AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY, 10);

	std::cout << "Success!" << std::endl;
	return 0;
}