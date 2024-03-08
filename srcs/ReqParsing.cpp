#include "ReqParsing.hpp"

ReqParsing::ReqParsing() {}

static std::string ContentFormat(std::string key);
static bool IsMethodAllowed(const Location &locat, std::string  method);
static std::string ExtractHeader(std::string const &request, std::string key);

ReqParsing::ReqParsing(const std::string &reqRaw, const ServerConfig &conf)
{
	std::istringstream iss(reqRaw.substr(0, reqRaw.find("\r\n")));
	std::string buff;
	
	while (std::getline(iss, buff, ' '))
	{
		if (buff == "GET" || buff == "POST" || buff == "DELETE")
			_method = buff;
		else if (buff[0] == '/')
			parsePath(buff, conf);
		else if (buff == HTTP_VERSION)
			_httpVersion = buff;
	}
	if (_method.empty() || _url.empty() || _httpVersion.empty())
		std::__throw_runtime_error("BAD REQUEST");
	if (_method == "GET")
		HandleGET();
	else if (_method == "POST")
		HandlePOST(reqRaw);
}

ReqParsing::~ReqParsing(void) {}

void ReqParsing::parsePath(std::string path, const ServerConfig &conf)
{
	std::string rootServer = conf.getRoot() + path;

	if (std::ifstream(rootServer.data()).good())
	{
		_statusCode = "200";
		std::map<std::string, Location>::const_iterator it = conf.getLocations().find(path);
		_url = conf.getRoot() + path;
		if (it != conf.getLocations().end())
		{
			if (IsMethodAllowed(it->second, _method) == false)
				std::__throw_runtime_error("NOT ALLOWED METHOD");
			_url = it->second.indexFiles.front();
			_contentType = it->second.indexFiles.front().substr(it->second.indexFiles.front().find_last_of(".") + 1);
		}	
		else if (ContentFormat(path.substr(path.find_last_of('.') + 1)).empty() == false)
			_contentType = path.substr(path.find_last_of('.') + 1);
	}
	else
	{
		// Assim que a estrutura de throw estiver funcionando mandar erro e setar o necessario lá pra q ele não passe pelo HandleGET()
		_statusCode = "404";
		_url = conf.getErrorPages().at(_statusCode);
		_contentType = conf.getErrorPages().at(_statusCode).substr(conf.getErrorPages().at(_statusCode).find_last_of('.') + 1);
	}
}

void ReqParsing::HandleGET()
{
	std::ifstream file;
	std::stringstream responseHead;
	std::stringstream responseBody;
	std::stringstream fullResponse;

	responseHead << HTTP_VERSION;
	if (_statusCode.find("200") != std::string::npos)
		responseHead << " " + _statusCode + " OK\r\n";
	else
		responseHead << " " + _statusCode + " Not Found\r\n";
	responseHead << "Content-Format: " << ContentFormat(_contentType) + "\r\n";
	file.open(_url.c_str(), std::ios::binary);
	if (file.fail())
	{
		std::cerr << "error open" << std::endl;
		return ;
	}
	responseBody << file.rdbuf();
	responseHead << "Content-Length: " << responseBody.str().size();
	responseHead << "\r\n\r\n";
	fullResponse <<  responseHead.str() << responseBody.str();
	_httpResponse = fullResponse.str();
}

void ReqParsing::HandlePOST(const std::string &rawRequest)
{
	std::string content = rawRequest.substr(rawRequest.rfind("\r\n\r\n") + 4);
	std::ofstream file;

	_httpResponse += (HTTP_VERSION);
	if (!ExtractHeader(rawRequest.substr(rawRequest.find("\r\n\r\n") + 4), "Content-Disposition").empty())
	{
		std::string form = ExtractHeader(rawRequest.substr(rawRequest.find("\r\n\r\n") + 4), "Content-Disposition");
		_url.append(form.substr(form.find("filename=\"") + 10));
		_url.erase(_url.find("\""));
	}
	file.open(_url.c_str(), std::ios::out | std::ios::binary);
	if (!file)
		std::__throw_runtime_error("error create archive");
	file.write(content.c_str(), content.size());
	file.close();
	_httpResponse += " " + _statusCode + " OK\r\n";
	_httpResponse += "\r\n";
}

static std::string ContentFormat(std::string key)
{
	std::map<std::string, std::string> contentTypes;
	contentTypes["txt"]  = "text/plain";
    contentTypes["html"] = "text/html";
    contentTypes["css"]  = "text/css";
    contentTypes["js"]   = "text/javascript";
    contentTypes["json"] = "application/json";
    contentTypes["xml"]  = "application/xml";
    contentTypes["pdf"]  = "application/pdf";
    contentTypes["zip"]  = "application/zip";
    contentTypes["gzip"] = "application/gzip";
    contentTypes["tar"]  = "application/x-tar";
    contentTypes["png"]  = "image/png";
    contentTypes["jpg"]  = "image/jpeg";
    contentTypes["jpeg"] = "image/jpeg";
    contentTypes["gif"]  = "image/gif";
    contentTypes["svg"]  = "image/svg+xml";
    contentTypes["ico"]  = "image/x-icon";
	std::map<std::string, std::string>::iterator it = contentTypes.find(key);
	if(it == contentTypes.end())
		return ("");
	return (contentTypes[key]);
}

static bool IsMethodAllowed(const Location &locat, std::string  method)
{
	for (std::vector<std::string>::const_iterator it =  locat.allowedMethods.begin(); it  != locat.allowedMethods.end(); it++)
	{
		if (it->compare(method) == 0)
			return (true);
	}
	return (false);
}

static std::string ExtractHeader(std::string const &request, std::string key)
{
	std::map<std::string, std::string> header;
	std::string inBuff;
	std::size_t startPos	= request.find('\n') + 1;
	std::size_t endPos		= request.find('\n', startPos + 1) + 1;
	while (request.compare(startPos - 2, 4, "\r\n\r\n") != 0)
	{
		inBuff = request.substr(startPos, endPos - startPos - 2);
		header.insert(std::make_pair(inBuff.substr(0, inBuff.find(':')), inBuff.substr(inBuff.find(':') + 2)));
		startPos	= request.find('\n', startPos + 1) + 1;
		endPos		= request.find('\n', startPos + 1) + 1;
	}
	std::map<std::string, std::string>::iterator it = header.find(key);
	if(it == header.end())
		return ("");
	return (header[key]);
}

std::string ReqParsing::getHttpResponse() {return (this->_httpResponse);}
