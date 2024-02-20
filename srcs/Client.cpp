#include "Client.hpp"

Client::Client(std::string rawRequest, const ServerConfig &conf)
{
	std::istringstream iss(rawRequest);
	std::string buff;


	std::getline(iss, buff, ' ');
	_method = buff;

	std::getline(iss,  buff, ' ');
	_path = conf.getRoot() + buff;

	if (std::ifstream(_path.data()).good())
	{
		_statusCode = "200";
		if (isDirectory(_path))
		{
			_isDir = true;
			std::map<std::string, Location>::const_iterator it = conf.getLocations().find(buff);
			if (it != conf.getLocations().end())
			{
				_path = it->second.indexFiles.front();
				_contentType = it->second.indexFiles.front().substr(it->second.indexFiles.front().find_last_of(".") + 1);	
			}
		}
		else if (ContentFormat(buff.substr(buff.find_last_of('.') + 1)).empty() == false)
		{
			_isDir = false;
			_contentType = buff.substr(buff.find_last_of('.') + 1);
		}
	}
	else
	{
		_statusCode = "404";
		_path = conf.getErrorPages().at(_statusCode);
		_contentType = conf.getErrorPages().at(_statusCode).substr(conf.getErrorPages().at(_statusCode).find_last_of('.') + 1);
	}
}

Client::~Client()
{
}

std::string Client::GenerateResponse()
{
	std::string		httpResponse;
	std::ifstream	file;

	httpResponse.append(HTTP_VERSION);
	if (_method.compare("GET") == 0)
	{
		if (_statusCode.find("200") != std::string::npos)
		{
			if (_isDir == true)
				httpResponse.append(TextHandler(_path));
			else
			{
				httpResponse.append("200 OK\r\n");
				if (ContentFormat(_contentType).find("text") != std::string::npos)
					httpResponse.append(TextHandler(this->_path));
				if (ContentFormat(_contentType).find("image") != std::string::npos)
					httpResponse.append(ImageHandler(this->_path));
			}
		}
		else
		{
			httpResponse.append("404 Not Found\n");
			httpResponse.append(TextHandler(_path));	
		}
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
	responseHead << "\n\n";
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
	responseHead << "\n\n";
	fullResponse <<  responseHead.str() << responseBody.str();
	return (fullResponse.str());
}

std::string Client::ContentFormat(std::string key)
{
	std::map<std::string, std::string> contentTypes;

	contentTypes["txt"]  = "text/plain";
    contentTypes["html"] = "text/html";
    contentTypes["css"]  = "text/css";
    contentTypes["js"]   = "text/javascript";
    contentTypes["json"] = "application/json";
    contentTypes["xml"]  = "application/xml";
    contentTypes["pdf"]  = "application/pdf";
    contentTypes["zip"]  = "application/zip";
    contentTypes["gzip"] = "application/gzip";
    contentTypes["tar"]  = "application/x-tar";
    contentTypes["png"]  = "image/png";
    contentTypes["jpg"]  = "image/jpeg";
    contentTypes["jpeg"] = "image/jpeg";
    contentTypes["gif"]  = "image/gif";
    contentTypes["svg"]  = "image/svg+xml";
    contentTypes["ico"]  = "image/x-icon";

	std::map<std::string, std::string>::iterator it = contentTypes.find(key);

	if(it == contentTypes.end())
		return ("");
	return (contentTypes[key]);
}

const std::string &Client::getPath() const
{
	return _path;
}

const std::string &Client::getMethod() const
{
	return _method;
}