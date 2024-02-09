#include "Client.hpp"

Client::Client(std::string rawRequest) : _rawRequest(rawRequest)
{
	std::istringstream iss(rawRequest);
	std::string token;

	
	std::getline(iss, token, ' ');
	_method = token;

	std::getline(iss,  token, ' ');
	_path = token;
}

Client::~Client()
{
}

std::string Client::GenerateResponse(const ServerConfig &conf)
{
	std::string responseBody;
	std::string fullPath(conf.getRoot() + _path);
	std::string buff;
	std::fstream file;

	responseBody.append("HTTP/1.1 ");
	if (_method.compare("GET") == 0)
	{
		if (std::ifstream(fullPath.data( )).good())
		{
			_statusCode = "200 OK\n";
			std::map<std::string, Location>::const_iterator it = conf.getLocations().find(_path);
			if (it != conf.getLocations().end())
			{
				file.open(it->second.indexFiles.front().c_str());
				if (file.fail())
				{
					std::cerr << "error open" << std::endl;
					return (0);
				}
			}
			else
				file.open(fullPath.c_str());
		}
		else
		{
			file.open(conf.getErrorPages().at("404").data());
			_statusCode  = "404 Not Found\n";
		}
	}
	responseBody.append(_statusCode);
	responseBody.append("Content-type: text/html\n");
	responseBody.append("Content-Length: 4096\n\n ");
	while (std::getline(file, buff))
		responseBody.append(buff + "\n");
	return (responseBody);
}

const std::string &Client::getPath() const
{
	return _path;
}

const std::string &Client::getMethod() const
{
	return _method;
}