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
		if (fcntl(socketVec[i].getSocketFd(), F_SETFL, O_NONBLOCK) < 0)
			std::__throw_runtime_error("Error set nonblocking I/O");
		this->_poll.addSocketFd(socket);
    }
    _launch(socketVec, configs);
}

void WebServer::_launch(SocketVec &socketVec, const ConfigVec &conf) 
{
	while (true) 
	{
				this->_poll.execute();
		for (size_t i = 0; i < this->_poll.getSize(); ++i)
		{
			if (this->_poll.checkEvent(i))
			{
				if (i < socketVec.size())
				{
					_newSock = socketVec[i].accept();
				}
			}
        	_read(conf[i]);
		}

    }
}

void WebServer::_read(const ServerConfig &conf) 
{
	char buff[4096] = {0};
	int bytesread;
	std::string Crequest;

	while ((bytesread = recv(_newSock, buff, sizeof(buff), 0)) > 0)
	{
		Crequest.append(buff, bytesread);
		if (Crequest.find("Expect: 100-continue") != std::string::npos) {
            sleep(2);
            continue;
        }
		if (Crequest.find("multipart/form-data") != std::string::npos)
		{
            std::string boundary;
            size_t      contentTypePos = Crequest.find("Content-Type: ");

            if (contentTypePos != std::string::npos) {
                size_t lineEndPos = Crequest.find("\r\n", contentTypePos);
                if (lineEndPos != std::string::npos) {
                    std::string contentTypeLine = Crequest.substr(contentTypePos, lineEndPos - contentTypePos);
                    size_t boundaryPos = contentTypeLine.find("boundary=");
                    if (boundaryPos != std::string::npos) {
                        boundary = contentTypeLine.substr(boundaryPos + 9);
                    }
                }
            }
            std::string terminatingBoundary = "\r\n--" + boundary + "--";
            if (Crequest.find(terminatingBoundary) != std::string::npos)
                break;
		}
		else if (Crequest.find("\r\n\r\n") != std::string::npos)
            break;
	}
	ReqParsing parsing(Crequest, conf);
	Response response(parsing);
	_respond(response);
}

void WebServer::_respond(Response response)
{
	int bytesreturned, totalbytes = 0;
	while ((size_t)totalbytes < response._response.size())
	{
		bytesreturned = send(_newSock, response._response.c_str(), response._response.size(), 0);
		std::cout << response._response << std::endl;
		if (bytesreturned < 0)
		{
			std::cerr << "fail send" << std::endl;
			return ;
		}
		if (bytesreturned == 0)
			break;
		totalbytes += bytesreturned;
	}
    close(_newSock);
}
