#include "Response.hpp"

Response::Response() {}

Response::Response(ReqParsing request) : 
	_request(request), 
	_serverRoot(request.getRoot() + request.getUrl()){
	checkError();
}

Response::~Response()
{
}

void Response::checkError()
{
	_response = HTTP_VERSION;
	if (_request.getHasError() == true){
		errorResponse(ResponseUtils::StatusCodes(_request.getErrorCode()));
		return ;
	}
	if (_request.getLocation() != NULL){
		if (ResponseUtils::IsMethodAllowed(*_request.getLocation(), _request.getMethod()) == false) {
			errorResponse(ResponseUtils::StatusCodes("405"));
			return ;
		}
		else{
			_serverRoot = _request.getLocation()->indexFiles.front();
		}
	}
	if (_request.getContentLength() > 0 && _request.getHasBody() == false){
		errorResponse(ResponseUtils::StatusCodes("100"));
		return ;
	}
	if (_request.getHasBody() == true && _request.getBody().size() > _request.getMaxBodySize()){
		errorResponse(ResponseUtils::StatusCodes("413"));
		return ;
	}
	if (ServerUtils::checkFileExist(_serverRoot) == false) {
		errorResponse(ResponseUtils::StatusCodes("404"));
		return ;
	}
	generateResponse();
}

void Response::generateResponse()
{
	if (_request.getMethod() == "GET")
		HandleGET();
	else if (_request.getMethod() == "POST")
		HandlePOST();
	else if (_request.getMethod() == "DELETE")
		HandleDELETE();
}

void Response::HandleGET() {
    std::ifstream file;
    std::stringstream responseHead;
    std::stringstream responseBody;
    std::stringstream fullResponse;
	std::string fileExtension = ServerUtils::getExtension(_serverRoot);

    file.open(_serverRoot.c_str(), std::ios::binary);
    if (file.fail()) {
		errorResponse(ResponseUtils::StatusCodes("500"));
		return ;
    }
	else{
    	responseBody << file.rdbuf();
	}
	responseHead << ResponseUtils::StatusCodes("200");
    responseHead << "Content-Format: " << ReqParsUtils::ContentFormat(fileExtension) << "\r\n";
    responseHead << "Content-Length: " << responseBody.str().size() << "\r\n";
	responseHead << "Date: " << ResponseUtils::getCurrDate() << "\r\n";
	responseHead << "Server: WebServer\r\n";
    responseHead << "\r\n";
    fullResponse << responseHead.str() << responseBody.str();
    _response += fullResponse.str();
}

void Response::HandlePOST() {
	std::stringstream httpResponse;
    std::ofstream file;


	if (_request.getFileName() != "")
		_serverRoot += _request.getFileName();
	else
		_serverRoot += "file";
	if (ServerUtils::checkFileExist(_serverRoot) == true)
		httpResponse << ResponseUtils::StatusCodes("204");
	else 
		httpResponse << ResponseUtils::StatusCodes("201");
	file.open(_serverRoot.c_str(), std::ios::out | std::ios::binary);
	if (!file) {
		errorResponse(ResponseUtils::StatusCodes("500"));
		return ;
	}
	else {
		file.write(_request.getBody().c_str(), _request.getBody().size());
		file.close();
	}
	httpResponse << "Date: " << ResponseUtils::getCurrDate() << "\r\n";
	httpResponse << "Server: WebServer\r\n";
	httpResponse << "\r\n";
	_response += httpResponse.str();
}

void Response::HandleDELETE()
{
	if (ServerUtils::isDirectory(_serverRoot))
		_response += ResponseUtils::StatusCodes("405");
	else if (ServerUtils::isFileReadable(_serverRoot) == false)
		_response += ResponseUtils::StatusCodes("403");
	else
	{
        std::remove(_serverRoot.c_str());
		_response += ResponseUtils::StatusCodes("200");
		_response += "Date: " + ResponseUtils::getCurrDate() + "\r\n";
		_response += "Server: WebServer\r\n";
		_response += "\r\n";
	}
}

void Response::errorResponse(std::string error)
{
	_response += error;
	_response += "Date: " + ResponseUtils::getCurrDate() + "\r\n";
	_response += "Server: WebServer\r\n";
	_response += "\r\n\r\n";
}
