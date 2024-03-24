#include "ReqParsing.hpp"

ReqParsing::ReqParsing() {}

ReqParsing::ReqParsing(const std::string &reqRaw, const ServerConfig &conf)
    : _root(conf.getRoot()),
      _body(""),
      _fileName(""),
      _contentLength(0),
      _chunkBody(false),
      _hasBody(false),
      _location(NULL) {
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
    } state;

    state = METHOD;

    while (iss >> buff) {
        switch (state) {
            case METHOD:
                if (buff != "GET" || buff != "POST" || buff != "DELETE") {
                    // throw error
                }
                _method = buff;
                state = URL;
                break;
            case URL:
                if (buff[0] == '/') {
                    _url = buff;
                    state = HTTP;
                }
                break;
            case HTTP:
                if (buff != HTTP_VERSION) {
                    // throw error
                }
                _httpVersion = buff;
                break;
        }
    }
}

void ReqParsing::extractHeaderInfo(const std::string &rawReq,
                                   const ServerConfig &conf) {
    if (ReqParsUtils::ExtractHeader(rawReq, "Transfer-Encoding") == "chunked")
        _chunkBody = true;
    if (ReqParsUtils::ExtractHeader(rawReq, "Content-Length") != "") {
        std::istringstream issCLength(
            ReqParsUtils::ExtractHeader(rawReq, "Content-Length"));
        std::istringstream issBSize(conf.getClientMaxBodySize());

        issBSize >> _maxBodySize;
        issCLength >> _contentLength;
        if (_contentLength > 0 && ReqParsUtils::hasBody(rawReq) == true) {
            _body = rawReq.substr(rawReq.rfind("\r\n\r\n") + 4);
            _hasBody = true;
        }
    }
    if (ReqParsUtils::ExtractHeader(rawReq, "Content-Type") != "")
        _contentType = ReqParsUtils::ExtractHeader(rawReq, "Content-Type");
    if (_contentType.find("multipart/form-data") != std::string::npos &&
        _hasBody == true) {
        std::string form = ReqParsUtils::ExtractHeader(
            rawReq.substr(rawReq.find("\r\n\r\n") + 4), "Content-Disposition");
        _fileName = form.substr(form.find("filename=\"") + 10);
        _fileName.erase(_fileName.find("\""));
    }
}

ReqParsing::~ReqParsing(void) {}

void ReqParsing::setLocation(const ServerConfig &conf) {
    std::string rootServer = _root + _url;

    if (ServerUtils::isDirectory(rootServer)) {
        std::map<std::string, Location>::const_iterator it =
            conf.getLocations().find(_url);
        if (it != conf.getLocations().end()) {
            _location = new Location();
            *_location = it->second;
        }
    }
}

const std::string &ReqParsing::getRoot() const { return (_root); }

const std::string &ReqParsing::getMethod() const { return (this->_method); }

const std::string &ReqParsing::getUrl() const { return (this->_url); }

const std::string &ReqParsing::getHttpVersion() const {
    return (this->_httpVersion);
}

const std::string &ReqParsing::getContentType() const {
    return (this->_contentType);
}

const std::string &ReqParsing::getTransferEncoding() const {
    return (this->_transferEncoding);
}

const std::string &ReqParsing::getBody() const { return (this->_body); }

const std::string &ReqParsing::getFileName() const { return (this->_fileName); }

int ReqParsing::getContentLength() const { return (this->_contentLength); }

int ReqParsing::getMaxBodySize() const { return (this->_maxBodySize); }

bool ReqParsing::getChunkBody() const { return (this->_chunkBody); }

bool ReqParsing::getHasBody() const { return (this->_hasBody); }

const Location *ReqParsing::getLocation() { return (_location); }
