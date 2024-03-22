#include "ReqParsing.hpp"

ReqParsing::ReqParsing() {}

ReqParsing::ReqParsing(const std::string &reqRaw, const ServerConfig &conf) : 
	_location(NULL),
	_root(conf.getRoot()),
	_chunkBody(false),
	_hasBody(false), 
	_contentLength(0),
	_body(""),
	_fileName(""){

	parsFirtsLine(reqRaw.substr(0, reqRaw.find("\r\n")));
	extractHeaderInfo(reqRaw, conf);
	setLocation(conf);
}

void ReqParsing::parsFirtsLine(const std::string &firstline) {
	std::istringstream iss(firstline);
	std::string buff;

	enum {
		METHOD,
		URL,
		HTTP,
	}state;

	state = METHOD;

	while (iss >> buff)
	{
		switch (state)
		{
			case METHOD:
				if (buff != "GET" || buff != "POST" || buff != "DELETE") {
					//throw error
				}
				else {
					_method = buff;
					state = URL;
				}
				break;
			case URL:
				if (buff[0] == '/'){
					_url = buff;
					state = HTTP;
				}
				break;
			case HTTP:
				if (buff != HTTP_VERSION) {
					//throw error
				}
				else
					_httpVersion = buff;
				break;
		}
	}
}

void ReqParsing::extractHeaderInfo(const std::string &rawReq, const ServerConfig &conf)
{
	if (ReqParsUtils::ExtractHeader(rawReq, "Transfer-Encoding:") == "chunked")
		_chunkBody = true;
	if (ReqParsUtils::ExtractHeader(rawReq, "Content-Length:") != "") {
		std::istringstream issCLength(ReqParsUtils::ExtractHeader(rawReq, "Content-Length:"));
		std::istringstream issBSize(conf.getClientMaxBodySize());

		issBSize >> _maxBodySize;
		issCLength >> _contentLength;
		if (_contentLength > 0 && ReqParsUtils::hasBody(rawReq) == true){
			_body = rawReq.substr(rawReq.rfind("\r\n\r\n") + 4);
			_hasBody = true;
		}
	}
	if (ReqParsUtils::ExtractHeader(rawReq, "Content-Type:") != "")
		_contentType = ReqParsUtils::ExtractHeader(rawReq, "Content-Type:");
	if (_contentType == "multipart/form-data" && _hasBody == true) {
		std::string form = ReqParsUtils::ExtractHeader(_body, "Content-Disposition:");
		_fileName = ReqParsUtils::ExtractHeader(form, "filename=\"" + 10);
		_fileName.erase(_fileName.find("\""));
	}
}

ReqParsing::~ReqParsing(void) {
	if (_location != NULL)
		delete _location;
}

void ReqParsing::setLocation(const ServerConfig &conf) {
    std::string rootServer = _root + _url;

    if (ServerUtils::isDirectory(rootServer)) {
        std::map<std::string, Location>::const_iterator it =
            conf.getLocations().find(_url);
        if (it != conf.getLocations().end()){
			_location = new Location();
            *_location = it->second;
		}
    }
}

const std::string ReqParsing::getRoot() { return (_root); }

std::string ReqParsing::getMethod() { return (this->_method); }

std::string ReqParsing::getUrl() { return (this->_url); }

std::string ReqParsing::getHttpVersion() { return (this->_httpVersion); }

std::string ReqParsing::getContentType() { return (this->_contentType); }

std::string ReqParsing::getTransferEncoding() { return (this->_transferEncoding); }

std::string ReqParsing::getBody() { return (this->_body); }

std::string ReqParsing::getFileName() { return (this->_fileName); }

int ReqParsing::getContentLength() { return (this->_contentLength); }

int ReqParsing::getMaxBodySize() { return (this->_maxBodySize); }

bool ReqParsing::getChunkBody() { return (this->_chunkBody); }

bool ReqParsing::getHasBody() { return (this->_hasBody); }

Location *ReqParsing::getLocation() { return (this->_location); }
