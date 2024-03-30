#pragma once

#include <dirent.h>

#include <iostream>
#include <sstream>

#include "Cgi.hpp"
#include "ReqParsing.hpp"
#include "ResponseUtils.hpp"
#include "ServerUtils.hpp"

class Response {
   public:
    std::string _response;
    Response();
    Response(ReqParsing request);
    ~Response();

    size_t getStatusCode() const;

   private:
    ReqParsing _request;
    std::string _serverRoot;
    std::size_t _statusCode;

    void checkError();
    void generateResponse();
    void _HandleGET();
    void _HandlePOST();
    void _HandleDELETE();
    const std::string _handleAutoindex(const std::string&);

    std::string setStatusCode(const std::string&);
};
