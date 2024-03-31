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
    std::string _queryUrl;
    std::string _httpVersion;
    std::string _contentType;
    std::string _transferEncoding;
    std::string _body;
    std::string _form;
    std::string _fileName;
    std::string _statusCode;
    std::string _connection;
    std::size_t _contentLength;
    std::size_t _maxBodySize;
    bool _hasBodyLimit;
    bool _chunkBody;
    bool _firtLineParsed;
    bool _headerParsed;
    bool _bodyParsed;
    bool _isParsed;
    Location _location;
    ServerConfig _server;
	std::map<std::string, std::string> _errorPagePath;

   public:
    ReqParsing();
    ReqParsing(const ServerConfig &server);
    ~ReqParsing();

    void parse(const std::string &rawReq, int clientRes);

    void setStatusCode(const std::string &statusCode);
    void setConnection(const std::string &connection);

    const std::string &getRoot() const;
    const std::string &getMethod() const;
    const std::string &getUrl() const;
    const std::string &getQueryUrl() const;
    const std::string &getHttpVersion() const;
    const std::string &getContentType() const;
    const std::string &getTransferEncoding() const;
    const std::string &getBody() const;
    const std::string &getForm() const;
    const std::string &getFileName() const;
    const std::string &getStatusCode() const;
    const std::string &getConnection() const;
    const std::size_t &getContentLength() const;
    const std::size_t &getMaxBodySize() const;
    bool getChunkBody() const;
    bool getHasBodyLimit() const;
    bool getIsParsed() const;
    const Location &getLocation() const;
    const ServerConfig &getServer() const;
	const std::map<std::string, std::string> &getErrorPagePath() const;

   private:
    void parsFirtsLine(const std::string &rawReq);
    void setLocation(const ServerConfig &server);
    void extractReqInfo(const std::string &rawReq, const ServerConfig &server);
    void parseBody(const std::string &reqRaw);
    void isMultiPart();
};
