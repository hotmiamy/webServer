#include "Poll.hpp"

Poll::Poll() : _pollfds(), _sockets() {}

Poll::~Poll() {}

void Poll::addFd(const Socket &socket,const int &fd)
{
	pollfd pfd;

	_sockets.push_back(socket);
	pfd.fd = fd;
	pfd.events = POLLIN | POLLOUT | POLLERR | POLLHUP;
	pfd.revents = 0;
	this->_pollfds.push_back(pfd);
}

void Poll::execute()
{
	int ret = poll(this->_pollfds.data(), this->_pollfds.size(), 0);
	checkerror(ret, std::runtime_error("poll error"));
}

void Poll::removeEventFd(const Socket &socket)
{
	std::vector<pollfd>::reverse_iterator itpoll;
    for (itpoll = _pollfds.rbegin(); itpoll != _pollfds.rend(); ++itpoll) {
        if (itpoll->fd == socket.getClientFd()) {
            ::close(itpoll->fd);
            _pollfds.erase(std::vector<pollfd>::iterator(&(*itpoll)));
            break;
        }
    }
	std::vector<Socket>::reverse_iterator itsocket;
    for (itsocket = _sockets.rbegin(); itsocket != _sockets.rend(); ++itsocket) {
        if (itsocket->getClientFd() == socket.getClientFd()) {
            _sockets.erase(std::vector<Socket>::iterator(&(*itsocket)));
            break;
        }
    }
}

bool Poll::checkEvent(size_t inx, long diff, bool &timeout)
{
	if (diff >= 30){
		timeout = true;
		return (true);
	}
	if (this->_pollfds[inx].revents & POLLHUP)
		return (true);
	if (this->_pollfds[inx].revents & POLLERR)
		return (true);
	if (this->_pollfds[inx].revents & POLLIN)
		return (true);
	if (this->_pollfds[inx].revents & POLLOUT)	
		return (true);
	return (false);
}

const std::vector<pollfd> &Poll::getEvent() const {return (this->_pollfds);}

std::size_t Poll::getSize() {return (this->_pollfds.size());}

const Socket &Poll::getSocket(size_t inx) const {return (_sockets[inx]);}

const int &Poll::getFd(int inx) const {return (this->_pollfds[inx].fd);}

void Poll::clearAllFds()
{
	for (size_t i = 0; i < this->_pollfds.size(); ++i){
		close(this->_pollfds[i].fd);
	}
	this->_pollfds.clear();
}

template <typename ExceptionType>
void Poll::checkerror(int ret, const ExceptionType &exception)
{
	if (ret < 0)
		throw exception;
}