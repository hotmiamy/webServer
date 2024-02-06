#include "WebServer.hpp"

WebServer::WebServer() :  _newSock(0) {}

WebServer::WebServer(const WebServer &other) { *this = other; }

WebServer &WebServer::operator=(const WebServer &other) {
    if (this != &other) 
	{
		_poll = other._poll;
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
    }
}

void WebServer::_read() 
{
	char buff[1024] = {0};
    if (recv(_newSock, buff, sizeof(buff), 0) < 0)
		std::cerr << "buff Empty" << std::endl;
	else
	{
    	std::cout << buff << '\n';
		Client request(buff);
		_respond(request.GenerateResponse());
	}
}

void WebServer::_respond(std::string response)
{
    send(_newSock, response.data(), response.size(), 0);
    close(_newSock);
}
