#include "WebServer.hpp"

WebServer::WebServer() : _buff(), _newSock(0) {}

WebServer::WebServer(const WebServer &other) { *this = other; }

WebServer &WebServer::operator=(const WebServer &other) {
    if (this != &other) {
        std::strcpy(_buff, other._buff);
        _newSock = other._newSock;
    }
    return *this;
}

WebServer::~WebServer() {}

void WebServer::run(const ConfigVec &configs) {
    SocketVec socketVec;

    for (ConfigVec::size_type i = 0; i < configs.size(); ++i) {
        Socket socket(configs.at(i));
        socket.connect();
        socketVec.push_back(socket);
		fcntl(socketVec[i].getSocketFd(), F_SETFL, O_NONBLOCK);
		this->_poll.addSocketFd(socket);
    }
    _launch(socketVec);
}

void WebServer::_launch(SocketVec &socketVec) 
{
	while (true) 
	{
		this->_poll.execute();
		for (size_t i = 0; i < this->_poll.getSize(); ++i)
		{
			if (this->_poll.checkEvent(i))
			{
				if (i < socketVec.size())
					_newSock = socketVec[i].accept();
			}
		}
        _read();
        _respond();
    }
}

void WebServer::_read() {
    bzero(_buff, sizeof(_buff));
    if (recv(_newSock, _buff, sizeof(_buff), 0) < 0)
		std::cerr << "buff Empty" << std::endl;
	else
    	std::cout << _buff << '\n';
}

void WebServer::_respond() {
	fcntl(_newSock, F_SETFL, O_NONBLOCK);
    send(_newSock, "teste", 6, 0);
    close(_newSock);
}
