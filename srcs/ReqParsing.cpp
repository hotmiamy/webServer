#include "ReqParsing.hpp"

ReqParsing::ReqParsing() {}

ReqParsing::ReqParsing(const std::string &reqRaw, const ServerConfig &conf) : 
	_root(conf.getRoot()),
	_body(""),
	_form(""),
	_fileName(""),
	_errorCode(""),
	_contentLength(0),
	_maxBodySize(0),
	_chunkBody(false),
	_location(){

	try
	{
		parsFirtsLine(reqRaw.substr(0, reqRaw.find("\r\n")));
		extractReqInfo(reqRaw, conf);
		parseBody();
		setLocation(conf);
	}
	catch(const std::exception& e)
	{
		_errorCode = e.what();
	}
}

void ReqParsing::parsFirtsLine(const std::string &firstline) {
    std::istringstream iss(firstline);
    std::string buff;

    enum {
        METHOD,
        URL,
        HTTP,
    } state;

    state = METHOD;

	while (iss >> buff)
	{
		switch (state)
		{
			case METHOD:
				if (buff != "GET" && buff != "POST" && buff != "DELETE") {
					throw std::runtime_error("501");
				}
				_method = buff;
				state = URL;
				break;
			case URL:
				if (buff[0] == '/'){
					_url = buff;
					state = HTTP;
				}
				break;
			case HTTP:
				if (buff != HTTP_VERSION) {
					throw std::runtime_error("505");
				}
				_httpVersion = buff;
				break;
		}
	}
}

void ReqParsing::extractReqInfo(const std::string &rawReq,
                                   const ServerConfig &conf) {
    if (ReqParsUtils::ExtractHeader(rawReq, "Transfer-Encoding") == "chunked")
        _chunkBody = true;
    if (ReqParsUtils::ExtractHeader(rawReq, "Content-Length") != "") {
        std::istringstream issCLength(
            ReqParsUtils::ExtractHeader(rawReq, "Content-Length"));
        std::istringstream issBSize(conf.getClientMaxBodySize());

        issBSize >> _maxBodySize;
        issCLength >> _contentLength;
    }
    if (ReqParsUtils::ExtractHeader(rawReq, "Content-Type") != ""){
        _contentType = ReqParsUtils::ExtractHeader(rawReq, "Content-Type");
	}
	ReqParsUtils::extractBody(rawReq, _body, _form);
    if (_contentType.find("multipart/form-data") != std::string::npos &&
        _form.empty() == false) {
        _fileName = _form.substr(_form.find("filename=\"") + 10);
        _fileName.erase(_fileName.find("\""));
    }
}

void ReqParsing::parseBody()
{
	std::string unparsed(_body);

	if (_chunkBody == true){
		_body = "";
		while (unparsed.size() > 0)
		{
			if (unparsed.find("\r\n0\r\n") == 0 || unparsed.find("0") == 0)
				break;
			std::string chunkSizeStr = unparsed.substr(0, unparsed.find("\r\n"));
			std::stringstream ss(chunkSizeStr);
			std::size_t       chunkSize = 0;
			ss >> std::hex >> chunkSize;

			unparsed.erase(0, chunkSizeStr.size() + 2);
			_body.append(unparsed.substr(0, chunkSize));
			unparsed.erase(0, chunkSize + 2);
		}
	}
}

ReqParsing::~ReqParsing(void) {}

void ReqParsing::setLocation(const ServerConfig &conf) {
    std::string rootServer = _root + _url;

    if (ServerUtils::isDirectory(rootServer)) {
        std::map<std::string, Location>::const_iterator it =
            conf.getLocations().find(_url);
        if (it != conf.getLocations().end()) {
            _location = it->second;
        }
    }
}

const std::string &ReqParsing::getRoot() const { return (_root); }

const std::string &ReqParsing::getMethod() const { return (this->_method); }

const std::string &ReqParsing::getUrl() const { return (this->_url); }

const std::string &ReqParsing::getHttpVersion() const { return (this->_httpVersion); }

const std::string &ReqParsing::getContentType() const { return (this->_contentType); }

const std::string &ReqParsing::getTransferEncoding() const { return (this->_transferEncoding); }

const std::string &ReqParsing::getBody() const { return (this->_body); }

const std::string &ReqParsing::getForm() const { return (this->_form); }

const std::string &ReqParsing::getFileName() const { return (this->_fileName); }

const std::string &ReqParsing::getErrorCode() const { return (this->_errorCode); }

const std::size_t &ReqParsing::getContentLength() const { return (this->_contentLength); }

const std::size_t &ReqParsing::getMaxBodySize() const { return (this->_maxBodySize); }

bool ReqParsing::getChunkBody() const { return (this->_chunkBody); }

bool ReqParsing::getHasBodyLimit() const { return (this->_hasBodyLimit); }

const Location &ReqParsing::getLocation() const { return (_location); }
