#include "Poll.hpp"

Poll::Poll() {}

Poll::~Poll() {}

void Poll::addSocketFd(Socket &socket)
{
	pollfd pfd;

	pfd.fd = socket.getSocketFd();
	pfd.events = POLLIN | POLLPRI | POLLOUT | POLLWRBAND;
	pfd.revents = 0;
	this->_pollfds.push_back(pfd);
}

void Poll::execute()
{
	int ret = poll(this->_pollfds.data(), this->_pollfds.size(), -1);
	checkerror(ret, std::runtime_error("poll error"));
}

bool Poll::checkEvent(size_t inx)
{
	if (this->_pollfds[inx].revents & POLLPRI)
		return (true);
	if (this->_pollfds[inx].revents & POLLIN)
		return (true);
	if (this->_pollfds[inx].revents & POLLWRBAND)
		return (true);
	if (this->_pollfds[inx].revents & POLLOUT)	
		return (true);
	return (false);
}

size_t Poll::getSize() const {return (this->_pollfds.size());}


template <typename ExceptionType>
void Poll::checkerror(int ret, const ExceptionType &exception)
{
	if (ret <= 0)
		throw exception;
}