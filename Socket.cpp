#include "Socket.hpp"

ft::Socket::Socket(std::string ip, std::string port) : ip(ip), port(port), _SockFd(-1), _ClientFd(-1)
{}

ft::Socket::~Socket()
{
	if (res)
		freeaddrinfo(res);
	if (_SockFd != -1)
		close(_SockFd);
}

void ft::Socket::socket()
{
	// cria uma struct addrinfo limpa
	struct addrinfo info = {};
	// SIM :)
	int yes = 1;

	// define o tipo de ip para ipv4 e ipv6
	info.ai_family = AF_UNSPEC;
	// define o tipo o protocolo para tcp/ip
	info.ai_socktype = SOCK_STREAM;

	// popula a struct addrinfo com os dados
	CheckConnection(getaddrinfo(ip.c_str(), port.c_str(), &info, &res));
	// gera o FD do socket
	_SockFd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	// impede o problema de "Endereço já em uso" permitendo o reuso do socket
	CheckConnection(setsockopt(_SockFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)));
}

void ft::Socket::bind()
{
	// abre as porteiras
	int bind = ::bind(_SockFd, res->ai_addr, res->ai_addrlen);
	CheckConnection(bind);
}

void ft::Socket::listen(int ConnectionQueue)
{
	// escuta uma porta aberta pelo bind
	int listen = ::listen(_SockFd, ConnectionQueue);
	CheckConnection(listen);
}

void ft::Socket::connect()
{
	// bind sempre primeiro que listen
	socket();
	bind();
	listen(10);
}

int ft::Socket::accept()
{
	// struct limpa para receber endereços do navegador
	struct sockaddr_storage ClientAddr = {};
	socklen_t ClientAddrSize = sizeof(ClientAddr);

	// fica aguardando o navegador se conectar
	_ClientFd = ::accept(_SockFd, (struct sockaddr *)&ClientAddr, &ClientAddrSize);
	CheckConnection(_ClientFd);
	return (_ClientFd);
}

void ft::Socket::CheckConnection(int connection)
{
	if (connection < 0)
	{
		std::cerr << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
}

struct addrinfo *ft::Socket::get_AddrRes()
{
	return res;
}

int ft::Socket::get_SockFd()
{
	return _SockFd;
}
