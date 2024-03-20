#pragma once

#include <iostream>
#include <map>
#include <sstream>

#include "ServerConfig.hpp"
#include "ServerUtils.hpp"
#include "ReqParsUtils.hpp"

#define HTTP_VERSION "HTTP/1.1"

class ReqParsing {
   private:
    std::string _method;
    std::string _url;
    std::string _httpVersion;
    std::string _statusCode;
    std::string _contentType;
    std::string _httpResponse;
    std::vector<std::string> _allowMethods;
	Location *_location;

   public:
    ReqParsing();
    ReqParsing(const std::string &rawReq, const ServerConfig &conf);
    ~ReqParsing();

    std::string getHttpResponse();

   private:
    void parsePath(std::string path, const ServerConfig &conf);
    void HandleGET();
    void HandlePOST(const std::string &rawRequest);
	void HandleDELETE();
};
