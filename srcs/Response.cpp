#include "Response.hpp"

Response::Response() : _statusCode(0) {}

Response::Response(ReqParsing request)
    : _errorPagePath(request.getRoot() + request.getUrl()), 
	_serverRoot(request.getRoot() + request.getUrl()), 
	_statusCode(0), 
	_request(request) {
	try
	{
		checkError();
		generateResponse();
	}
	catch(const std::exception& e)
	{
		_HandleErrorPage(e.what());		
	}
}

Response::~Response() {}

void Response::checkError() {
    _response = HTTP_VERSION;
    if (_request.getStatusCode().empty() == false)
        throw std::runtime_error(setStatusCode(_request.getStatusCode()));
    if (_request.getLocation().empty() == false) {
        if (ResponseUtils::IsMethodAllowed(_request.getLocation(),
                                           _request.getMethod()) == false)
            throw std::runtime_error(setStatusCode("405"));
        _serverRoot = _request.getLocation().indexFile;
    }
    if (_request.getHasBodyLimit() == true) {
        if (_request.getBody().empty() == false &&
            _request.getBody().length() > _request.getMaxBodySize())
            throw std::runtime_error(setStatusCode("413"));
    }
    if (!_serverRoot.empty() && !ServerUtils::fileExists(_serverRoot)) {
        throw std::runtime_error(setStatusCode("404"));
    }
}

void Response::generateResponse() {
    if (_request.getMethod() == "GET") {
        return _HandleGET();
    }
    if (_request.getMethod() == "POST") {
        return _HandlePOST();
    }
    if (_request.getMethod() == "DELETE") {
        return _HandleDELETE();
    }
}

void Response::_HandleGET() {
    std::ifstream file;
    std::stringstream responseHead, responseBody, fullResponse;
    std::string fileExtension = ServerUtils::getExtension(_serverRoot);

    if (!_request.getLocation().empty() &&
        _request.getLocation().redirectionSet()) {
        std::string code = _request.getLocation().redirection.first,
                    redirectedURL = _request.getLocation().redirection.second;
        file.open(_request.getUrl().c_str(), std::ios::binary);
        fullResponse << ResponseUtils::StatusCodes(code)
                     << "Location: " << redirectedURL << "\r\n\r\n";
        _response += fullResponse.str();
        return;
    }

    std::string filePath = _request.getRoot() + _request.getUrl();
    if (ServerUtils::isDirectory(filePath) &&
        _request.getLocation().autoindex) {
        if (_request.getLocation().autoindex) {
            responseHead << ResponseUtils::StatusCodes("200")
                         << "Content-Format: "
                         << ReqParsUtils::ContentFormat("html") << "\r\n";
            responseBody << _handleAutoindex(filePath);
        }
    } else {
        if (_request.getUrl().find(".py") != std::string::npos) {
            Cgi cgi = Cgi(_request, "GET");
            cgi.execute();
            responseBody << cgi.getOut();
            responseHead << ResponseUtils::StatusCodes("200");
            responseHead << "Content-Format: "
                         << ReqParsUtils::ContentFormat("txt") << "\r\n";
        } else {
            file.open(_serverRoot.c_str(), std::ios::binary);
            if (file.fail()) {
                throw std::runtime_error(setStatusCode("500"));
            }
            responseBody << file.rdbuf();
            responseHead << ResponseUtils::StatusCodes("200");
            responseHead << "Content-Format: "
                         << ReqParsUtils::ContentFormat(fileExtension)
                         << "\r\n";
        }
    }
    responseHead << "Content-Length: " << responseBody.str().size() << "\r\n";
    responseHead << "Date: " << ResponseUtils::getCurrDate() << "\r\n";
    responseHead << "Server: WebServer\r\n";
    responseHead << "\r\n";
    fullResponse << responseHead.str() << responseBody.str();
    _response += fullResponse.str();
}

void Response::_HandlePOST() {
    std::stringstream responseHead, responseBody, fullResponse;
    std::ofstream file;

    if (_request.getUrl().find("post.py") != std::string::npos) {
        Cgi cgi = Cgi(_request, "POST");
        cgi.execute();
        std::string fileExtension = ServerUtils::getExtension(_serverRoot);
        responseBody << cgi.getOut();
        responseHead << ResponseUtils::StatusCodes("200");
        responseHead << "Content-Format: "
                     << ReqParsUtils::ContentFormat(fileExtension) << "\r\n";
        responseHead << "Content-Length: " << responseBody.str().size()
                     << "\r\n";
    } else {
        if (_request.getBody().empty() == true)
            throw std::runtime_error(
                ResponseUtils::StatusCodes(setStatusCode("204")));
        _serverRoot += ResponseUtils::genFileName(_request);
        if (ServerUtils::fileExists(_serverRoot) == true) {
            throw std::runtime_error(setStatusCode("409"));
        }
        responseHead << ResponseUtils::StatusCodes("201");
        file.open(_serverRoot.c_str(), std::ios::out | std::ios::binary);
        if (!file) {
            throw std::runtime_error(setStatusCode("500"));
        }
        file.write(_request.getBody().c_str(), _request.getBody().size());
        file.close();
    }
    responseHead << "Date: " << ResponseUtils::getCurrDate() << "\r\n";
    responseHead << "Server: WebServer\r\n";
    responseHead << "\r\n";
    fullResponse << responseHead.str() << responseBody.str();
    _response += fullResponse.str();
}

void Response::_HandleDELETE() {
    if (_request.getQueryUrl().empty() == false) {
        _serverRoot +=
            _request.getQueryUrl().substr(_request.getQueryUrl().find("=") + 1);
    }
    if (ServerUtils::isDirectory(_serverRoot)) {
        throw std::runtime_error(setStatusCode("405"));
    }
    if (ServerUtils::isFileReadable(_serverRoot) == false) {
        throw std::runtime_error(setStatusCode("403"));
    }
    if (ServerUtils::isFileReadable(_serverRoot) == false) {
        throw std::runtime_error(setStatusCode("403"));
    }
    std::remove(_serverRoot.c_str());
    _response += ResponseUtils::StatusCodes("200");
    _response += "Date: " + ResponseUtils::getCurrDate() + "\r\n";
    _response += "Server: WebServer\r\n";
    _response += "\r\n";
}

const std::string Response::_handleAutoindex(const std::string& path) {
    std::stringstream ss;

    DIR* dir = opendir(path.c_str());
    dirent* entry;

    ss << "<h1>Index of " << _request.getLocation().path << "</h1>\n<ul>";

    while ((entry = readdir(dir))) {
        ss << "<li><a href=\"";
        ss << _request.getLocation().path;
        if (ss.str().at(ss.str().size() - 1) != '/') {
            ss << "/";
        }
        ss << entry->d_name;
        if (entry->d_type == DT_DIR) {
            ss << "/";
        }
        ss << "\">" << entry->d_name;
        if (entry->d_type == DT_DIR) {
            ss << "/";
        }
        ss << "</a></li>\n";
    }
    ss << "</ul></body></html>";
    closedir(dir);
    return ss.str();
}

void Response::_HandleErrorPage(std::string errorCode) {
	 std::stringstream responseHead, responseBody, fullResponse;

	_response = HTTP_VERSION;
	this->_errorPagePath = _request.getErrorPagePath().at(errorCode);
	if (this->_errorPagePath.empty() != true) { // << já verificar antes se o path existe
		if (ServerUtils::fileExists(_errorPagePath) == true) {
			std::ifstream file(_errorPagePath.c_str());
			responseBody << file.rdbuf();
			responseHead << ResponseUtils::StatusCodes(errorCode);
			responseHead << "Content-Format: "
						 << ReqParsUtils::ContentFormat("html") << "\r\n";
			responseHead << "Content-Length: " << responseBody.str().size() << "\r\n";
		}
		else
			responseHead << ResponseUtils::StatusCodes(errorCode);
		responseHead << "Connection: close\r\n";
		responseHead << "Date: " + ResponseUtils::getCurrDate() + "\r\n";
    	responseHead << "Server: WebServer\r\n";
    	responseHead << "\r\n\r\n";
	}
	fullResponse << responseHead.str() << responseBody.str();
	_response += fullResponse.str();
}

std::string Response::setStatusCode(const std::string& code) {
    std::istringstream iss(code);
    iss >> _statusCode;
    return code;
}

size_t Response::getStatusCode() const { return _statusCode; }
