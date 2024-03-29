#include "Response.hpp"

Response::Response() {}

Response::Response(ReqParsing request)
    : _request(request), _serverRoot(request.getRoot() + request.getUrl()) {
    checkError();
}

Response::~Response() {}

void Response::checkError() {
    _response = HTTP_VERSION;
    if (!(_request.getErrorCode().empty())) {
        return errorResponse(
            ResponseUtils::StatusCodes(_request.getErrorCode()));
    }
    if (!(_request.getLocation().empty())) {
        if (!(ResponseUtils::IsMethodAllowed(_request.getLocation(),
                                             _request.getMethod()))) {
            return errorResponse(ResponseUtils::StatusCodes("405"));
        }
        _serverRoot = _request.getLocation().indexFile;
    }
    if (_request.getHasBodyLimit()) {
        if (!_request.getBody().empty() &&
            _request.getBody().length() > _request.getMaxBodySize()) {
            return errorResponse(ResponseUtils::StatusCodes("413"));
        }
    }
    if (!_serverRoot.empty() && !ServerUtils::fileExists(_serverRoot)) {
        return errorResponse(ResponseUtils::StatusCodes("404"));
    }
    generateResponse();
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
    errorResponse(ResponseUtils::StatusCodes("405"));
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

    if (_request.getUrl().find("script.py") != std::string::npos) {
        Cgi cgi = Cgi(_request, "GET");
        cgi.execute();
        responseBody << cgi.getOut();
    } else {
        file.open(_serverRoot.c_str(), std::ios::binary);
        if (file.fail()) {
            return errorResponse(ResponseUtils::StatusCodes("500"));
        }
        responseBody << file.rdbuf();
    }

    responseHead << ResponseUtils::StatusCodes("200");
    responseHead << "Content-Format: "
                 << ReqParsUtils::ContentFormat(fileExtension) << "\r\n";
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
        responseHead << ResponseUtils::StatusCodes("200");
        responseHead << "Content-Format: "
                     << ReqParsUtils::ContentFormat(fileExtension) << "\r\n";
        responseHead << "Content-Length: " << responseBody.str().size()
                     << "\r\n";
    } else {
        _serverRoot += ResponseUtils::genFileName(_request);
        if (ServerUtils::fileExists(_serverRoot) == true)
            responseHead << ResponseUtils::StatusCodes("204");
        else
            responseHead << ResponseUtils::StatusCodes("201");
        file.open(_serverRoot.c_str(), std::ios::out | std::ios::binary);
        if (!file) {
            errorResponse(ResponseUtils::StatusCodes("500"));
            return;
        } else {
            file.write(_request.getBody().c_str(), _request.getBody().size());
            file.close();
        }
    }
    responseHead << "Date: " << ResponseUtils::getCurrDate() << "\r\n";
    responseHead << "Server: WebServer\r\n";
    responseHead << "\r\n\n";
    fullResponse << responseHead.str() << responseBody.str();
    _response += fullResponse.str();
}

void Response::HandleDELETE() {
    if (ServerUtils::isDirectory(_serverRoot))
        _response += ResponseUtils::StatusCodes("405");
    else if (ServerUtils::isFileReadable(_serverRoot) == false)
        _response += ResponseUtils::StatusCodes("403");
    else {
        std::remove(_serverRoot.c_str());
        _response += ResponseUtils::StatusCodes("200");
        _response += "Date: " + ResponseUtils::getCurrDate() + "\r\n";
        _response += "Server: WebServer\r\n";
        _response += "\r\n";
    }
}

void Response::errorResponse(std::string error) {
    _response += error;
    _response += "Date: " + ResponseUtils::getCurrDate() + "\r\n";
    _response += "Server: WebServer\r\n";
    _response += "\r\n\r\n";
}
