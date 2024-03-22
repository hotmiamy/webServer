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
	const std::string _root;
    std::string _method;
    std::string _url;
    std::string _httpVersion;
    std::string _contentType;
	std::string _transferEncoding;
	std::string _body;
	std::string _fileName;
	int 		_contentLength;
	int			_maxBodySize;
	bool		_chunkBody;
	bool		_hasBody;
	Location *_location;

   public:
    ReqParsing();
    ReqParsing(const std::string &rawReq, const ServerConfig &conf);
    ~ReqParsing();


	const std::string getRoot();
	std::string getMethod();
	std::string getUrl();
	std::string getHttpVersion();
	std::string getContentType();
	std::string getTransferEncoding();
	std::string getBody();
	std::string getFileName();
	int			getContentLength();
	int			getMaxBodySize();
	bool 		getHasBody();
	bool 		getChunkBody();
	Location	*getLocation();

   private:
   	void parsFirtsLine(const std::string &rawReq);
    void setLocation(const ServerConfig &conf);
   	void extractHeaderInfo(const std::string &rawReq, const ServerConfig &conf);
};
