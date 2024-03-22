#include "Response.hpp"

Response::Response(ReqParsing request) : 
	_request(request), 
	_serverRoot(request.getRoot() + request.getUrl()){
	genarateResponse();
}

Response::~Response()
{
}


void Response::genarateResponse()
{
	if (_request.getLocation() != NULL){
		if (ResponseUtils::IsMethodAllowed(*_request.getLocation(), _request.getMethod()) == false)
			_response = ResponseUtils::StatusCodes("405");
		else{
			_serverRoot = _request.getLocation()->indexFiles.front();
		}
	}
	if (_request.getContentLength() > 0 && _request.getHasBody() == false)
		_response = ResponseUtils::StatusCodes("100");
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

	responseHead << HTTP_VERSION;
    file.open(_serverRoot.c_str(), std::ios::binary);
    if (file.fail()) {
		// throw error 500
    }
	else{
    	responseBody << file.rdbuf();
	}
	responseHead << ResponseUtils::StatusCodes("200");
    responseHead << "Content-Format: " << ReqParsUtils::ContentFormat(_request.getLocation()->indexFiles.front()) + "\r\n";
    responseHead << "Content-Length: " << responseBody.str().size();
    responseHead << "\r\n\r\n";
    fullResponse << responseHead.str() << responseBody.str();
    _response = fullResponse.str();
}

void Response::HandlePOST() {
	std::stringstream httpResponse;
    std::ofstream file;


    httpResponse << HTTP_VERSION;
	if (_request.getFileName() != "")
		_serverRoot += _request.getFileName();
	else
		_serverRoot += "file";
	file.open(_serverRoot.c_str(), std::ios::out | std::ios::binary);
	if (!file) {
		//throw error 500
	}
	else {
		file.write(_request.getBody().c_str(), _request.getBody().size());
		file.close();
	}
	httpResponse << ResponseUtils::StatusCodes("200");
	httpResponse << "Content-Type: " << _request.getContentType() << "\r\n";
	httpResponse << "Content-Length: " << _request.getBody().size() << "\r\n";
	httpResponse << "Server: WebServer";
	httpResponse << "\r\n\r\n";
	_response = httpResponse.str();
}
void Response::HandleDELETE()
{
	_response  = HTTP_VERSION;
	if (ServerUtils::isDirectory(_serverRoot))
		_response += ResponseUtils::StatusCodes("405");
	else if (ServerUtils::isFileReadable(_serverRoot) == false)
		_response += ResponseUtils::StatusCodes("403");
	else
	{
        std::remove(_serverRoot.c_str());
		_response += ResponseUtils::StatusCodes("200");
		_response += "\r\n\r\n";
	}
}