#include "ReqParsUtils.hpp"

namespace ReqParsUtils {

std::string ContentFormat(std::string key) {
    std::map<std::string, std::string> contentTypes;

    contentTypes["txt"] = "text/plain";
    contentTypes["html"] = "text/html";
    contentTypes["css"] = "text/css";
    contentTypes["js"] = "text/javascript";
    contentTypes["json"] = "application/json";
    contentTypes["xml"] = "application/xml";
    contentTypes["pdf"] = "application/pdf";
    contentTypes["zip"] = "application/zip";
    contentTypes["gzip"] = "application/gzip";
    contentTypes["tar"] = "application/x-tar";
    contentTypes["png"] = "image/png";
    contentTypes["jpg"] = "image/jpeg";
    contentTypes["jpeg"] = "image/jpeg";
    contentTypes["gif"] = "image/gif";
    contentTypes["svg"] = "image/svg+xml";
    contentTypes["ico"] = "image/x-icon";

    std::map<std::string, std::string>::iterator it = contentTypes.find(key);

    if (it == contentTypes.end()) return ("application/octet-stream");
    return (contentTypes[key]);
}

std::string ExtractHeader(std::string const &request, std::string key) {
    std::map<std::string, std::string> header;
    std::string inBuff;
    std::size_t startPos = request.find('\n') + 1;
    std::size_t endPos = request.find('\n', startPos + 1) + 1;

    while (request.compare(startPos - 2, 4, "\r\n\r\n") != 0) {
        inBuff = request.substr(startPos, endPos - startPos - 2);
        header.insert(std::make_pair(inBuff.substr(0, inBuff.find(':')),
                                     inBuff.substr(inBuff.find(':') + 2)));
        startPos = request.find('\n', startPos + 1) + 1;
        endPos = request.find('\n', startPos + 1) + 1;
    }
	
    std::map<std::string, std::string>::iterator it = header.find(key);

    if (it == header.end()) return ("");
    return (header[key]);
}

}