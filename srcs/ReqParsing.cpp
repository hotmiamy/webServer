#include "ReqParsing.hpp"

ReqParsing::ReqParsing() {}

ReqParsing::ReqParsing(const std::string &reqRaw, const ServerConfig &conf) {
    std::istringstream iss(reqRaw.substr(0, reqRaw.find("\r\n")));
    std::string buff;

    while (std::getline(iss, buff, ' ')) {
        if (buff == "GET" || buff == "POST" || buff == "DELETE")
            _method = buff;
        else if (buff[0] == '/')
            parsePath(buff, conf);
        else if (buff == HTTP_VERSION)
            _httpVersion = buff;
    }
    if (_method.empty() || _url.empty() || _httpVersion.empty())
        std::__throw_runtime_error("BAD REQUEST");
    else if (_method == "GET")
        HandleGET();
    else if (_method == "POST")
        HandlePOST(reqRaw);
	else if (_method == "DELETE")
		HandleDELETE();
}

ReqParsing::~ReqParsing(void) {}

void ReqParsing::parsePath(std::string path, const ServerConfig &conf) {
    std::string rootServer = conf.getRoot() + path;

    if (ServerUtils::checkFileExist(rootServer)) {
        _statusCode = "200";
        std::map<std::string, Location>::const_iterator it =
            conf.getLocations().find(path);
        _url = conf.getRoot() + path;
        if (it != conf.getLocations().end()) {
            if (ReqParsUtils::IsMethodAllowed(it->second, _method) == false) 
                std::__throw_runtime_error("NOT ALLOWED METHOD");
			else
			{
            	_url = it->second.indexFiles.front();
            	_contentType =
            	    ServerUtils::getExtension(it->second.indexFiles.front());
			}
        } else if (!ReqParsUtils::ContentFormat(ServerUtils::getExtension(path)).empty()) {
            _contentType = ServerUtils::getExtension(path);
        }
    } else {
        // Assim que a estrutura de throw estiver funcionando mandar erro e
        // setar o necessario la pra q ele nao passe pelo HandleGET()
        _statusCode = "404";
        _url = conf.getErrorPages().at(_statusCode);
        _contentType =
            ServerUtils::getExtension(conf.getErrorPages().at(_statusCode));
    }
}

void ReqParsing::HandleGET() {
    std::ifstream file;
    std::stringstream responseHead;
    std::stringstream responseBody;
    std::stringstream fullResponse;

	responseHead << HTTP_VERSION;
    file.open(_url.c_str(), std::ios::binary);
    if (file.fail()) {
        responseHead << ReqParsUtils::StatusCodes("500");
    }
	else
	{
		responseHead << ReqParsUtils::StatusCodes(_statusCode);
    	responseBody << file.rdbuf();
	}
    responseHead << "Content-Format: " << ReqParsUtils::ContentFormat(_contentType) + "\r\n";
    responseHead << "Content-Length: " << responseBody.str().size();
    responseHead << "\r\n\r\n";
    fullResponse << responseHead.str() << responseBody.str();
    _httpResponse = fullResponse.str();
}

void ReqParsing::HandlePOST(const std::string &rawRequest) {
    std::string content = rawRequest.substr(rawRequest.rfind("\r\n\r\n") + 4);
    std::ofstream file;

    _httpResponse += HTTP_VERSION;
    if (!ReqParsUtils::ExtractHeader(rawRequest.substr(rawRequest.find("\r\n\r\n") + 4),
                       "Content-Disposition")
             .empty()) {
        std::string form =
            ReqParsUtils::ExtractHeader(rawRequest.substr(rawRequest.find("\r\n\r\n") + 4),
                          "Content-Disposition");
        _url.append(form.substr(form.find("filename=\"") + 10));
        _url.erase(_url.find("\""));
    }
    file.open(_url.c_str(), std::ios::out | std::ios::binary);
    if (!file) 
		_httpResponse += ReqParsUtils::StatusCodes("500");
	else
	{
    	_httpResponse += ReqParsUtils::StatusCodes(_statusCode);
    	file.write(content.c_str(), content.size());
	}
    file.close();
}

void ReqParsing::HandleDELETE()
{
	_httpResponse  = HTTP_VERSION;
	if (ServerUtils::isDirectory(_url))
		_httpResponse += ReqParsUtils::StatusCodes("405");
	else if (ServerUtils::isFileReadable(_url) == false)
		_httpResponse += ReqParsUtils::StatusCodes("403");
	else
	{
        std::remove(_url.c_str());
		_httpResponse += ReqParsUtils::StatusCodes(_statusCode);
		_httpResponse += "\r\n\r\n";
	}
}

std::string ReqParsing::getHttpResponse() { return (this->_httpResponse); }
