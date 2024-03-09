#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>

#include "ServerConfig.hpp"

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

   public:
    ReqParsing();
    ReqParsing(const std::string &rawReq, const ServerConfig &conf);
    ~ReqParsing();

    std::string getHttpResponse();

   private:
    void parsePath(std::string path, const ServerConfig &conf);
    std::string geneHandler();
    void HandleGET();
    void HandlePOST(const std::string &rawRequest);
};
