#pragma once

#include <iostream>
#include <map>
#include <sstream>

#include "ReqParsUtils.hpp"
#include "ServerConfig.hpp"
#include "ServerUtils.hpp"

#define HTTP_VERSION "HTTP/1.1"

class ReqParsing {
   private:
    std::string _root;
    std::string _method;
    std::string _url;
    std::string _httpVersion;
    std::string _contentType;
    std::string _transferEncoding;
    std::string _body;
    std::string _fileName;
	std::string _errorCode;
    std::size_t _contentLength;
    std::size_t _maxBodySize;
    bool _chunkBody;
    Location _location;

   public:
    ReqParsing();
    ReqParsing(const std::string &rawReq, const ServerConfig &conf);
    ~ReqParsing();

    const std::string &getRoot() const;
    const std::string &getMethod() const;
    const std::string &getUrl() const;
    const std::string &getHttpVersion() const;
    const std::string &getContentType() const;
    const std::string &getTransferEncoding() const;
    const std::string &getBody() const;
    const std::string &getFileName() const;
	const std::string &getErrorCode() const;
	const std::size_t &getContentLength() const ;
    const std::size_t &getMaxBodySize() const;
    bool getChunkBody() const;
    const Location &getLocation() const;

   private:
    void parsFirtsLine(const std::string &rawReq);
    void setLocation(const ServerConfig &conf);
    void extractHeaderInfo(const std::string &rawReq, const ServerConfig &conf);
};
