#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#define ROOT_PATH "server_root"

class Client
{
	private:
		std::string _rawRequest;
		std::string _path;
		std::string _method;
		std::string _statusCode;
	public:
		Client(std::string rawRequest);
		~Client();

		std::string GenerateResponse();

		const std::string &getPath() const;
		const std::string &getMethod() const;
};
