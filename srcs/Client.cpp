#include "Client.hpp"

Client::Client(std::string rawRequest, const ServerConfig &conf)
{
	ReqParsing parsing(rawRequest, conf);
}

Client::~Client()
{
}
/*
std::string Client::GenerateResponse()
{
	std::string		httpResponse;
	std::ifstream	file;

	httpResponse.append(HTTP_VERSION);
	if (_method.compare("GET") == 0)
	{
		
	}
	else if (_method.compare("POST") == 0)
	{
		httpResponse.append("200 OK\r\n");
		httpResponse.append("\r\n\r\n");
	}
	return (httpResponse);
}

std::string Client::TextHandler(std::string FullPath)
{
	std::stringstream responseHead;
	std::stringstream responseBody;
	std::stringstream fullResponse;
	std::ifstream file;
	std::string buff;

	responseHead << ContentFormat(_contentType) + "\r\n";
	file.open(FullPath.data(), std::ios::binary);
	if (file.fail())
	{
		std::cerr << "error open" << std::endl;
		return (0);
	}
	while (std::getline(file, buff))
		responseBody << buff + "\n";
	responseHead << "Content-Length: " << responseBody.str().size();
	responseHead << "\r\n\r\n";
	fullResponse <<  responseHead.str() << responseBody.str();
	return (fullResponse.str());
}

std::string Client::ImageHandler(std::string fullPath)
{
	std::ifstream file;
	std::stringstream responseHead;
	std::stringstream responseBody;
	std::stringstream fullResponse;
	
	responseHead << ContentFormat(_contentType) + "\r\n";
	file.open(fullPath.data(), std::ios::binary);
	if (file.fail())
	{
		std::cerr << "error open" << std::endl;
		return (0);
	}
	responseBody << file.rdbuf();
	responseHead << "Content-Length: " << responseBody.str().size();
	responseHead << "\r\n\r\n";
	fullResponse <<  responseHead.str() << responseBody.str();
	return (fullResponse.str());
}
 */
const std::string &Client::getPath() const
{
	return _path;
}

const std::string &Client::getMethod() const
{
	return _method;
}