#include "ReqParsing.hpp"

ReqParsing::ReqParsing() {}

ReqParsing::ReqParsing(const std::string &reqRaw, const ServerConfig &conf) : _location(NULL) {
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
	{
        _statusCode = "400";
		_httpResponse = HTTP_VERSION;
		_httpResponse += ReqParsUtils::StatusCodes(_statusCode);
		_httpResponse += "\r\n"; 
	}
    else if (_method == "GET")
        HandleGET();
    else if (_method == "POST")
        HandlePOST(reqRaw);
	else if (_method == "DELETE")
		HandleDELETE();
}

ReqParsing::~ReqParsing(void) {
	if (_location != NULL)
		delete _location;
}

void ReqParsing::parsePath(std::string path, const ServerConfig &conf) {
    std::string rootServer = conf.getRoot() + path;

    if (ServerUtils::checkFileExist(rootServer)) {
        _statusCode = "200";
        std::map<std::string, Location>::const_iterator it =
            conf.getLocations().find(path);
        _url = rootServer;
        if (it != conf.getLocations().end()){
			_location = new Location();
            *_location = it->second;
		}
        else if (!ReqParsUtils::ContentFormat(ServerUtils::getExtension(path)).empty()) {
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

	if (_location != NULL) {
		if (ReqParsUtils::IsMethodAllowed(*_location, _method) == false){
        	_statusCode = "405";
		}
		else {
        	_url = _location->indexFiles.front();
        	_contentType =
        	ServerUtils::getExtension(_location->indexFiles.front());
		}
	}
	responseHead << HTTP_VERSION;
    file.open(_url.c_str(), std::ios::binary);
    if (file.fail()) {
		 // Assim que a estrutura de throw estiver funcionando mandar erro e
        // setar o necessario la como a pagina de error aonde está o arquivo 500;
       _statusCode = "500";
    }
	else{
    	responseBody << file.rdbuf();
	}
	responseHead << ReqParsUtils::StatusCodes(_statusCode);
    responseHead << "Content-Format: " << ReqParsUtils::ContentFormat(_contentType) + "\r\n";
    responseHead << "Content-Length: " << responseBody.str().size();
    responseHead << "\r\n\r\n";
    fullResponse << responseHead.str() << responseBody.str();
    _httpResponse = fullResponse.str();
}

void ReqParsing::HandlePOST(const std::string &rawRequest) {
	std::stringstream httpResponse;
	std::size_t bodyPos = rawRequest.rfind("\r\n\r\n") + 4;
    std::string content = rawRequest.substr(bodyPos);
    std::ofstream file;

    httpResponse << HTTP_VERSION;
	if (_location != NULL) {
		if (ReqParsUtils::IsMethodAllowed(*_location, _method) == false){
        	_statusCode = "405";
		}
		else {
        	_url = _location->indexFiles.front();
        	_contentType =
        	ServerUtils::getExtension(_location->indexFiles.front());
		}
	}
	if (!content.empty()){
		if (!ReqParsUtils::ExtractHeader(rawRequest.substr(bodyPos), "Content-Disposition").empty()) {
		    std::string form =
		        ReqParsUtils::ExtractHeader(rawRequest.substr(bodyPos), "Content-Disposition");
		    _url.append(form.substr(form.find("filename=\"") + 10));
		    _url.erase(_url.find("\""));
		}
		file.open(_url.c_str(), std::ios::out | std::ios::binary);
		if (!file) 
			_statusCode = "500";
		else
		{
			file.write(content.c_str(), content.size());
			file.close();
		}
	}
	httpResponse << ReqParsUtils::StatusCodes(_statusCode);
	httpResponse << "Content-Type: " << ReqParsUtils::ExtractHeader(rawRequest, "Content-Type") << "\r\n";
	httpResponse << "Content-Length: " << content.size() << "\r\n";
	httpResponse << "Server: WebServer";
	httpResponse << "\r\n\r\n";
	_httpResponse = httpResponse.str();
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
