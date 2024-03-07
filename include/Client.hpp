#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include <iterator>
#include "ServerConfig.hpp"
#include "ServerUtils.hpp"
#include "ReqParsing.hpp"

#define HTTP_VERSION "HTTP/1.1"

class Client
{
	private:
		std::string	_path;
		std::string	_method;
		std::string	_contentType;
		std::string _statusCode;
		bool		_isDir;

	public:
		Client(std::string rawRequest, const ServerConfig &conf);
		~Client();

		std::string GenerateResponse();
		std::string TextHandler(std::string FullPath);
		std::string ImageHandler(std::string fullPath);

		std::string ContentFormat(std::string key);

		const std::string &getPath() const;
		const std::string &getMethod() const;
};
