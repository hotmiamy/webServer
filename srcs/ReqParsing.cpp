#include "ReqParsing.hpp"

ReqParsing::ReqParsing() {}

ReqParsing::ReqParsing(const ServerConfig &server) :
	_root(server.getRoot()),
	_queryUrl(""),
	_body(""),
	_form(""),
	_fileName(""),
	_statusCode(""),
	_connection(""),
	_contentLength(0),
	_maxBodySize(0),
	_hasBodyLimit(false),
	_chunkBody(false),
	_firtLineParsed(false),
	_headerParsed(false),
	_bodyParsed(false),
	_isParsed(false),
	_location(),
	_server(server){}

void ReqParsing::parse(const std::string &reqRaw, int clientRes)
{
	try
	{
		if (_firtLineParsed == false){
		parsFirtsLine(reqRaw.substr(0, reqRaw.find("\r\n")));
		setLocation(_server);
		if (_firtLineParsed == false)
			return ;
		}
		if (_headerParsed == false){
			extractReqInfo(reqRaw, _server);
			if (_headerParsed == false)
				return ;
		}
		if (_bodyParsed== false && clientRes > 0)
		{
			parseBody(reqRaw);
			isMultiPart();
			if (_bodyParsed == false)
				return ;
		}
		if (_firtLineParsed == true && _headerParsed == true && _bodyParsed == true)
			_isParsed = true;
	}
	catch(const std::exception& e)
	{
		setStatusCode(e.what());
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
					if (buff.find("?") != std::string::npos){
						_url = buff.substr(0, buff.find("?"));
						_queryUrl = buff.substr(buff.find("?") + 1);
					}
					else
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
	_firtLineParsed = true;
}

void ReqParsing::extractReqInfo(const std::string &rawReq,
                                   const ServerConfig &conf) {
	if (rawReq.find("\r\n\r\n") == std::string::npos)
		throw std::runtime_error("400");
	if (_method == "POST"){
		if (ReqParsUtils::ExtractHeader(rawReq, "Transfer-Encoding") == "chunked"){
			_chunkBody = true;
		}
		else if (ReqParsUtils::ExtractHeader(rawReq, "Content-Length") != "") {
			std::istringstream issCLength(
				ReqParsUtils::ExtractHeader(rawReq, "Content-Length"));
			std::istringstream issBSize(conf.getClientMaxBodySize());
			issBSize >> _maxBodySize;
			issCLength >> _contentLength;
		}
		else
			throw std::runtime_error("411");
	}
	if (ReqParsUtils::ExtractHeader(rawReq, "Content-Type") != "") {
		_contentType = ReqParsUtils::ExtractHeader(rawReq, "Content-Type");
	}
	if (ReqParsUtils::ExtractHeader(rawReq, "Connection") != "") {
        _connection = ReqParsUtils::ExtractHeader(rawReq, "Connection");
	}
	_headerParsed = true;
}

void ReqParsing::parseBody(const std::string &reqRaw)
{
	std::string unparsedBody(reqRaw.substr(reqRaw.find("\r\n\r\n") + 4));
	if (_method == "GET" || _method == "DELETE"){
		_bodyParsed = true;
		return ;
	}
	if (_chunkBody == true){
		while (unparsedBody.size() > 0)
		{
			std::string chunkSizeStr = unparsedBody.substr(0, unparsedBody.find("\r\n"));
			std::stringstream ss(chunkSizeStr);
			std::size_t       chunkSize = 0;
			ss >> std::hex >> chunkSize;

			unparsedBody.erase(0, chunkSizeStr.size() + 2);
			_body.append(unparsedBody.substr(0, chunkSize));
			unparsedBody.erase(0, chunkSize + 2);
			if (unparsedBody.find("\r\n0\r\n") == 0 || unparsedBody.find("0") == 0 || chunkSize == 0){
				_bodyParsed = true;
				break;
			}
		}
	}
	else if (_contentLength > 0){
		if (unparsedBody.size() > _maxBodySize)
			throw std::runtime_error("413");
	
		_body = unparsedBody.substr(0, _contentLength);
		unparsedBody.erase(0, _contentLength);
		_bodyParsed = true;
		return;
	}
}

void ReqParsing::isMultiPart()
{
	if (_bodyParsed == true && _contentType.find("multipart/form-data") != std::string::npos){
		std::size_t endboundaryPos = _body.find("\r\n\r\n");
		_form = _body.substr(0, endboundaryPos);
		_body.erase(0, endboundaryPos + 4);
		if (_form.empty() == false) {
    		_fileName = _form.substr(_form.find("filename=\"") + 10);
    		_fileName.erase(_fileName.find("\""));
   		}
	}
}

ReqParsing::~ReqParsing(void) {}

void ReqParsing::setLocation(const ServerConfig &server) {
    std::string rootServer = _root + _url;

    if (ServerUtils::isDirectory(rootServer)) {
        std::map<std::string, Location>::const_iterator it =
            server.getLocations().find(_url);
        if (it != server.getLocations().end()) {
            _location = it->second;
        }
    }
}

void ReqParsing::setStatusCode(const std::string &statusCode) { _statusCode = statusCode; }

void ReqParsing::setConnection(const std::string &connection) { _connection = connection; }

const std::string &ReqParsing::getRoot() const { return (_root); }

const std::string &ReqParsing::getMethod() const { return (this->_method); }

const std::string &ReqParsing::getUrl() const { return (this->_url); }

const std::string &ReqParsing::getQueryUrl() const { return (this->_queryUrl); }

const std::string &ReqParsing::getHttpVersion() const { return (this->_httpVersion); }

const std::string &ReqParsing::getContentType() const { return (this->_contentType); }

const std::string &ReqParsing::getTransferEncoding() const { return (this->_transferEncoding); }

const std::string &ReqParsing::getBody() const { return (this->_body); }

const std::string &ReqParsing::getForm() const { return (this->_form); }

const std::string &ReqParsing::getFileName() const { return (this->_fileName); }

const std::string &ReqParsing::getStatusCode() const { return (this->_statusCode); }

const std::string &ReqParsing::getConnection() const { return (this->_connection); }

const std::size_t &ReqParsing::getContentLength() const { return (this->_contentLength); }

const std::size_t &ReqParsing::getMaxBodySize() const { return (this->_maxBodySize); }

bool ReqParsing::getChunkBody() const { return (this->_chunkBody); }

bool ReqParsing::getHasBodyLimit() const { return (this->_hasBodyLimit); }

bool ReqParsing::getIsParsed() const { return (this->_isParsed); }

const Location &ReqParsing::getLocation() const { return (_location); }
