#include "WebServer.hpp"

WebServer::WebServer() {}

WebServer::WebServer(const WebServer &other) { *this = other; }

WebServer &WebServer::operator=(const WebServer &other) {
    if (this != &other) 
	{
		_poll = other._poll;
    }
    return *this;
}

WebServer::~WebServer() {
	_poll.clearAllFds();
}

void WebServer::init(char **argv) {

	_server = ServerConfig::fromFile(argv[1]);

    for (ServerVec::size_type i = 0; i < _server.size(); ++i) {
	        Socket socket(_server.at(i));
        int serverFD = socket.connect();
		if (fcntl(serverFD, F_SETFL, O_NONBLOCK) < 0)
			std::__throw_runtime_error("Error set nonblocking I/O");
		this->_poll.addFd(socket, serverFD);
		_server[i].setSocketFD(serverFD);
    }
    _launch();
}

void WebServer::_launch()
{
	while (true)
	{
		try
		{
			this->_poll.execute();
			for (size_t i = 0; i < this->_poll.getSize(); ++i)
			{
				std::time_t now;
				std::time(&now);
				long diff = 0;

				if (_poll.getFd(i) != _server[i].getSocketFD()){
					diff = std::difftime(now, _keepAlive[this->_poll.getSocket(i).getClientFd()]);
				}
				if (this->_poll.checkEvent(i, diff, _timeout))
				{
					Socket event = this->_poll.getSocket(i);
					if (_poll.getFd(i) == _server[i].getSocketFD())
					{
						event.accept(_server[i].getSocketFD());
						std::cout << "New connection on fd: " << event.getClientFd() << std::endl;
						if (_keepAlive.find(event.getClientFd()) == _keepAlive.end()) {
							_keepAlive.insert(std::pair<int, time_t>(event.getClientFd(), now));
						}
						if (fcntl(event.getClientFd(), F_SETFL, O_NONBLOCK) < 0)
							std::__throw_runtime_error("Error set nonblocking I/O");
						_poll.addFd(event, event.getClientFd());
					}
					else{
        				int clientRes = _read(event);
						if (clientRes > 0 || _timeout == true){
							_respond(event, clientRes);
						}
					}
				}
			}
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
}

int WebServer::_read(Socket &client)
{
	return(client.read(_rawRequest));
}

void WebServer::_respond(Socket &client, int clientRes)
{
	std::time_t now;
	std::time(&now);
	ReqParsing req(getCurrentServer(client));


	if (_timeout == true) {
		_timeout = false;
		if (_reqs[client.getClientFd()].getConnection().find("keep-alive") != std::string::npos) {
			std::cout << "Response on Fd: " << client.getClientFd();
			req.setStatusCode("204");
			req.setConnection("Connection: close\r\n");
			Response res(req);
			client.send(res._response);
			_keepAlive.erase(client.getClientFd());
			_reqs.erase(client.getClientFd());
			_poll.removeEventFd(client);
			return ;
		}
		else{
			std::cout << "Response on Fd " << client.getClientFd();
			req.setStatusCode("408");
			req.setConnection("Connection: close\r\n");
			Response res(req);
			_reqs.erase(client.getClientFd());
			_keepAlive.erase(client.getClientFd());
			client.send(res._response);
			_poll.removeEventFd(client);
			return ;
		}
	}
	std::cout << "Request of FD: " << client.getClientFd();
	std::cout << SEPARATOR << _rawRequest.substr(0, _rawRequest.find("\r\n\r\n")) << SEPARATOR;
	req.parse(_rawRequest, clientRes);
	if (_reqs.find(client.getClientFd()) == _reqs.end()){
		_reqs[client.getClientFd()] = req;
	}
	if ((req.getIsParsed() == true || req.getStatusCode().empty() == false)) {
		Response res(req);

		std::cout << "Response to connection on Fd " << client.getClientFd();
		_reqs.erase(client.getClientFd());
		client.send(res._response);
		_rawRequest = "";
	}
}

const ServerConfig &WebServer::getCurrentServer(const Socket &socket)
{
	for (size_t i = 0; i < _server.size();  ++i) {
		if (_server[i].getSocketFD() == socket.getServerFD()) {
			return _server[i];
		}
	}
	throw std::runtime_error("Socket not found");
}
