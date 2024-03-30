#include "Response.hpp"

Response::Response() {}

Response::Response(ReqParsing request)
    : _request(request), _serverRoot(request.getRoot() + request.getUrl()) {
	try
	{
		checkError();
		generateResponse();
	}
	catch(const std::exception& e)
	{
		_response = HTTP_VERSION;
		_response += e.what();
		_response += _request.getConnection();
		_response += "Date: " + ResponseUtils::getCurrDate() + "\r\n";
    	_response += "Server: WebServer\r\n";
    	_response += "\r\n\r\n";
	}
}

Response::~Response() {}

void Response::checkError()
{
	_response = HTTP_VERSION;
	if (_request.getStatusCode().empty() == false)
		throw std::runtime_error(ResponseUtils::StatusCodes(setStatusCode(_request.getStatusCode())));
	if (_request.getLocation().empty() == false){
		if (ResponseUtils::IsMethodAllowed(_request.getLocation(), _request.getMethod()) == false)
			throw std::runtime_error(ResponseUtils::StatusCodes(setStatusCode("405")));
		_serverRoot = _request.getLocation().indexFile;
	}
	if (_request.getHasBodyLimit() == true) {
		if (_request.getBody().empty() == false && _request.getBody().length() > _request.getMaxBodySize())
			throw std::runtime_error(ResponseUtils::StatusCodes(setStatusCode("413")));
	}
	if (!_serverRoot.empty() && !ServerUtils::fileExists(_serverRoot)) {
		throw std::runtime_error(ResponseUtils::StatusCodes(setStatusCode("404")));
	}
}
void Response::generateResponse() {
    if (_request.getMethod() == "GET") {
        return HandleGET();
    }
    if (_request.getMethod() == "POST") {
        return HandlePOST();
    }
    if (_request.getMethod() == "DELETE") {
        return HandleDELETE();
    }
}

void Response::HandleGET() {
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

    // if (false) {  // then, check folder and handle autoindex
    //     return;
    // }

    if (_request.getUrl().find(".py") != std::string::npos) {
        Cgi cgi = Cgi(_request, "GET");
        cgi.execute();
        responseBody << cgi.getOut();
		responseHead << ResponseUtils::StatusCodes(setStatusCode("200"));
		responseHead << "Content-Format: "
					<< ReqParsUtils::ContentFormat("txt") << "\r\n";
    } else {
        file.open(_serverRoot.c_str(), std::ios::binary);
        if (file.fail())
            throw std::runtime_error(ResponseUtils::StatusCodes(setStatusCode("500")));
        responseBody << file.rdbuf();
		responseHead << ResponseUtils::StatusCodes(setStatusCode("200"));
		responseHead << "Content-Format: "
					<< ReqParsUtils::ContentFormat(fileExtension) << "\r\n";
    }
    responseHead << "Content-Length: " << responseBody.str().size() << "\r\n";
    responseHead << "Date: " << ResponseUtils::getCurrDate() << "\r\n";
    responseHead << "Server: WebServer\r\n";
    responseHead << "\r\n";
    fullResponse << responseHead.str() << responseBody.str();
    _response += fullResponse.str();
}

void Response::HandlePOST() {
    std::stringstream responseHead, responseBody, fullResponse;
    std::ofstream file;

    if (_request.getUrl().find("post.py") != std::string::npos) {
        Cgi cgi = Cgi(_request, "POST");
        cgi.execute();
        std::string fileExtension = ServerUtils::getExtension(_serverRoot);
        responseBody << cgi.getOut();
		responseHead << ResponseUtils::StatusCodes(setStatusCode("200"));
		responseHead << "Content-Format: "
                 << ReqParsUtils::ContentFormat(fileExtension) << "\r\n";
    	responseHead << "Content-Length: " << responseBody.str().size() << "\r\n";
    } else {
		if (_request.getBody().empty()== true)
			throw std::runtime_error(ResponseUtils::StatusCodes(setStatusCode("204")));
        _serverRoot += ResponseUtils::genFileName(_request);
		if (ServerUtils::fileExists(_serverRoot) == true)
			throw std::runtime_error(ResponseUtils::StatusCodes(setStatusCode("409")));
		else 
			responseHead << ResponseUtils::StatusCodes(setStatusCode("201"));
		file.open(_serverRoot.c_str(), std::ios::out | std::ios::binary);
		if (!file)
			throw std::runtime_error(ResponseUtils::StatusCodes(setStatusCode("500")));
		else {
			file.write(_request.getBody().c_str(), _request.getBody().size());
			file.close();
		}
	}
    responseHead << "Date: " << ResponseUtils::getCurrDate() << "\r\n";
    responseHead << "Server: WebServer\r\n";
    responseHead << "\r\n";
    fullResponse << responseHead.str() << responseBody.str();
    _response += fullResponse.str();
}

void Response::HandleDELETE() {
	if (_request.getQueryUrl().empty() == false)
		_serverRoot += _request.getQueryUrl().substr(_request.getQueryUrl().find("=") + 1);
    if (ServerUtils::isDirectory(_serverRoot))
        throw std::runtime_error(ResponseUtils::StatusCodes(setStatusCode("405")));
    else if (ServerUtils::isFileReadable(_serverRoot) == false)
        throw std::runtime_error(ResponseUtils::StatusCodes(setStatusCode("403")));
    else {
        std::remove(_serverRoot.c_str());
        _response += ResponseUtils::StatusCodes(setStatusCode("200"));
        _response += "Date: " + ResponseUtils::getCurrDate() + "\r\n";
        _response += "Server: WebServer\r\n";
        _response += "\r\n";
    }
}

std::string Response::setStatusCode(std::string code) {
	std::istringstream iss(code);
	iss >> _statusCode;
	return code;
}

const std::size_t &Response::getStatusCode() const {
	return _statusCode;
}
