#include "DirectiveHandler.hpp"

const std::string DirectiveHandler::ERR_LISTEN =
    "error at 'listen' directive: ";
const std::string DirectiveHandler::ERR_SERVER_NAME =
    "error at 'server_name' directive: ";
const std::string DirectiveHandler::ERR_ERROR_PAGE =
    "error at 'error_page' directive: ";
const std::string DirectiveHandler::ERR_LOCATION =
    "error at 'location' directive: ";
const std::string DirectiveHandler::ERR_ALLOWED_METHODS =
    "error at 'allowed_methods' directive: ";

DirectiveHandler::DirectiveHandler() {
    _directiveMap["listen"] = &DirectiveHandler::_handleListenDirective;
    _directiveMap["server_name"] =
        &DirectiveHandler::_handleServerNameDirective;
    _directiveMap["error_page"] = &DirectiveHandler::_handleErrorPageDirective;
    _directiveMap["location"] = &DirectiveHandler::_handleLocationDirective;
}

DirectiveHandler::~DirectiveHandler() {}

void DirectiveHandler::_handleListenDirective(std::istringstream &iss,
                                              ServerConfig &cfg) {
    std::string port;

    if (!(iss >> port) || !_isNumeric(port)) {
        throw std::runtime_error(ERR_LISTEN +
                                 "the port should be a numeric value");
    }
    if (iss.rdbuf()->in_avail() != 0) {
        throw std::runtime_error(
            ERR_LISTEN + "there should be one and only one port per server");
    }
    cfg.setPort(port);
}

void DirectiveHandler::_handleServerNameDirective(std::istringstream &iss,
                                                  ServerConfig &cfg) {
    std::string server;

    if (!(iss >> server)) {
        throw std::runtime_error(ERR_SERVER_NAME +
                                 "at least one server name should be provided");
    }
    cfg.addServer(server);
    while (iss >> server) {
        cfg.addServer(server);
    }
}

void DirectiveHandler::_handleErrorPageDirective(std::istringstream &iss,
                                                 ServerConfig &cfg) {
    std::string errorCode, path;

    if (!(iss >> errorCode) || !_isNumeric(errorCode)) {
        throw std::runtime_error(ERR_ERROR_PAGE +
                                 "the error code should be a numeric value");
    }
    if (!(iss >> path) || !_isFileReadable(path)) {
        throw std::runtime_error(ERR_ERROR_PAGE +
                                 "page should point to a valid path");
    }
    if (iss.rdbuf()->in_avail() != 0) {
        throw std::runtime_error(
            ERR_ERROR_PAGE +
            "there shouldn't exist any more args for this directive, just "
            "<error_code> <error_page>");
    }

    cfg.addErrorPage(errorCode, path);
}

bool DirectiveHandler::_isNumeric(const std::string &str) const {
    return str.find_last_not_of("0123456789") == std::string::npos;
}

bool DirectiveHandler::_isFileReadable(const std::string &path) const {
    struct stat fileInfo;

    return stat(path.c_str(), &fileInfo) == 0 &&
           (S_ISREG(fileInfo.st_mode) && fileInfo.st_mode & S_IRUSR);
}

void DirectiveHandler::_handleLocationDirective(std::istringstream &iss,
                                                ServerConfig &cfg) {
    Location location;
    std::string line;
    while (std::getline(iss, line)) {
        std::istringstream lineIss(line);
        if (!location.pathSet()) {
            if (!(lineIss >> location.path) || location.path == "{") {
            }
            std::string tmp;
            if (!(lineIss >> tmp) || tmp != "{") {
            }
        }
        if (line.find("index") != std::string::npos &&
            !location.indexFilesSet()) {
            lineIss.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
            _handleIndexFiles(lineIss, location);
        }
        if (line.find("allowed_methods") != std::string::npos &&
            !location.allowedMethodsSet()) {
            lineIss.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
            _handleAllowedMethodsDirective(lineIss, location);
        }
    }
}

void DirectiveHandler::_handleAllowedMethodsDirective(std::istringstream &iss,
                                                      Location &location) {
    std::string httpMethod;

    if (!(iss >> httpMethod) || !_isAllowedHttpMethod(httpMethod)) {
        throw std::runtime_error(
            ERR_ALLOWED_METHODS +
            "at least one allowed method should be specified");
    }
    location.allowedMethods.push_back(httpMethod);

    while (iss >> httpMethod) {
        if (!_isAllowedHttpMethod(httpMethod)) {
            throw std::runtime_error(ERR_ALLOWED_METHODS + httpMethod +
                                     " is not allowed");
        }
        location.allowedMethods.push_back(httpMethod);
    }
}

bool DirectiveHandler::_isAllowedHttpMethod(const std::string &str) const {
    return str == "GET" || str == "POST";
}

void DirectiveHandler::_handleIndexFiles(std::istringstream &iss,
                                         Location &location) {
    std::string file;
    while (iss >> file) {
        location.indexFiles.push_back(file);
    }
}

void DirectiveHandler::process(const std::string &directive,
                               std::istringstream &iss, ServerConfig &cfg) {
    if (_directiveMap.find(directive) != _directiveMap.end()) {
        (this->*_directiveMap[directive])(iss, cfg);
    }
}
