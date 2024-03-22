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
		Response(ReqParsing request);
		~Response();

		void genarateResponse();
		void HandleGET();
		void HandlePOST();
		void HandleDELETE();
};
