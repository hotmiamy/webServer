#pragma once

#include <iostream>
#include <sstream>

#include "Cgi.hpp"
#include "ReqParsing.hpp"
#include "ResponseUtils.hpp"
#include "ServerUtils.hpp"

class Response {
   private:
    ReqParsing _request;
    std::string _serverRoot;

   public:
    Response();
    Response(ReqParsing request);
    ~Response();

    std::string _response;

    void generateResponse();
    void checkError();
    void HandleGET();
    void HandlePOST();
    void HandleDELETE();
    void errorResponse(std::string error);
};
