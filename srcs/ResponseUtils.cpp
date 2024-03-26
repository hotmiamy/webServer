#include "ResponseUtils.hpp"

namespace ResponseUtils {

std::string StatusCodes(const std::string &code) {
    std::map<std::string, std::string> statusCodes;

    statusCodes["100"] = " " + code + " Continue\r\n";
    statusCodes["101"] = " " + code + " Switching Protocols\r\n";
    statusCodes["102"] = " " + code + " Processing\r\n";
    statusCodes["200"] = " " + code + " OK\r\n";
    statusCodes["201"] = " " + code + " Created\r\n";
    statusCodes["202"] = " " + code + " Accepted\r\n";
    statusCodes["203"] = " " + code + " Non-Authoritative Information\r\n";
    statusCodes["204"] = " " + code + " No Content\r\n";
    statusCodes["205"] = " " + code + " Reset Content\r\n";
    statusCodes["206"] = " " + code + " Partial Content\r\n";
    statusCodes["207"] = " " + code + " Multi-Status\r\n";
    statusCodes["208"] = " " + code + " Already Reported\r\n";
    statusCodes["226"] = " " + code + " IM Used\r\n";
    statusCodes["300"] = " " + code + " Multiple Choices\r\n";
    statusCodes["301"] = " " + code + " Moved Permanently\r\n";
    statusCodes["302"] = " " + code + " Found\r\n";
    statusCodes["303"] = " " + code + " See Other\r\n";
    statusCodes["304"] = " " + code + " Not Modified\r\n";
    statusCodes["305"] = " " + code + " Use Proxy\r\n";
    statusCodes["307"] = " " + code + " Temporary Redirect\r\n";
    statusCodes["308"] = " " + code + " Permanent Redirect\r\n";
    statusCodes["400"] = " " + code + " Bad Request\r\n";
    statusCodes["401"] = " " + code + " Unauthorized\r\n";
    statusCodes["402"] = " " + code + " Payment Required\r\n";
    statusCodes["403"] = " " + code + " Forbidden\r\n";
    statusCodes["404"] = " " + code + " Not Found\r\n";
    statusCodes["405"] = " " + code + " Method Not Allowed\r\n";
    statusCodes["406"] = " " + code + " Not Acceptable\r\n";
    statusCodes["407"] = " " + code + " Proxy Authentication Required\r\n";
    statusCodes["408"] = " " + code + " Request Timeout\r\n";
    statusCodes["409"] = " " + code + " Conflict\r\n";
    statusCodes["410"] = " " + code + " Gone\r\n";
    statusCodes["411"] = " " + code + " Length Required\r\n";
    statusCodes["412"] = " " + code + " Precondition Failed\r\n";
    statusCodes["413"] = " " + code + " Payload Too Large\r\n";
    statusCodes["414"] = " " + code + " URI Too Long\r\n";
    statusCodes["415"] = " " + code + " Unsupported Media Type\r\n";
    statusCodes["416"] = " " + code + " Range Not Satisfiable\r\n";
    statusCodes["417"] = " " + code + " Expectation Failed\r\n";
    statusCodes["418"] = " " + code + " I'm a teapot\r\n";
    statusCodes["421"] = " " + code + " Misdirected Request\r\n";
    statusCodes["422"] = " " + code + " Unprocessable Entity\r\n";
    statusCodes["423"] = " " + code + " Locked\r\n";
    statusCodes["424"] = " " + code + " Failed Dependency\r\n";
    statusCodes["425"] = " " + code + " Too Early\r\n";
    statusCodes["426"] = " " + code + " Upgrade Required\r\n";
    statusCodes["428"] = " " + code + " Precondition Required\r\n";
    statusCodes["429"] = " " + code + " Too Many Requests\r\n";
    statusCodes["431"] = " " + code + " Request Header Fields Too Large\r\n";
    statusCodes["451"] = " " + code + " Unavailable For Legal Reasons\r\n";
    statusCodes["500"] = " " + code + " Internal Server Error\r\n";
    statusCodes["501"] = " " + code + " Not Implemented\r\n";
    statusCodes["502"] = " " + code + " Bad Gateway\r\n";
    statusCodes["503"] = " " + code + " Service Unavailable\r\n";
    statusCodes["504"] = " " + code + " Gateway Timeout\r\n";
    statusCodes["505"] = " " + code + " HTTP Version Not Supported\r\n";
    statusCodes["506"] = " " + code + " Variant Also Negotiates\r\n";
    statusCodes["507"] = " " + code + " Insufficient Storage\r\n";
    statusCodes["508"] = " " + code + " Loop Detected\r\n";
    statusCodes["510"] = " " + code + " Not Extended\r\n";
    statusCodes["511"] = " " + code + " Network Authentication Required\r\n";

    std::map<std::string, std::string>::const_iterator it =
        statusCodes.find(code);
    if (it == statusCodes.end()) return ("");
    return (statusCodes[code]);
}

bool IsMethodAllowed(const Location &locat, const std::string &method) {
    for (std::vector<std::string>::const_iterator it =
             locat.allowedMethods.begin();
         it != locat.allowedMethods.end(); it++) {
        if (it->compare(method) == 0) return (true);
    }
    return (false);
}

std::string getCurrDate(void) {
    time_t now;
    std::tm *local_time;
    char buffer[64];

    now = std::time(NULL);
    local_time = std::localtime(&now);
    std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %T %Z", local_time);

    return (buffer);
}

}  // namespace ResponseUtils
