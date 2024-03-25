#pragma once

#include <iostream>
#include <sstream>
#include "ReqParsing.hpp"
#include "ResponseUtils.hpp"
#include "ServerUtils.hpp"

class Response
{
	private:
		ReqParsing _request;
		std::string _serverRoot;

	public:
		std::string _response;

	public:
		Response();
		Response(ReqParsing request);
		~Response();

		void generateResponse();
		void checkError();
		void HandleGET();
		void HandlePOST();
		void HandleDELETE();
		void errorResponse(std::string error);

};
