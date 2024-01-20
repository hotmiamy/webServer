#include "DirectiveHandler.hpp"

const std::string DirectiveHandler::ERR_LISTEN =
    "error at 'listen' directive: ";
const std::string DirectiveHandler::ERR_SERVER_NAME =
    "error in 'server_name' directive: ";
const std::string DirectiveHandler::ERR_ERROR_PAGE =
    "error in 'error_page' directive: ";
const std::string DirectiveHandler::ERR_LOCATION =
    "error in 'location' directive: ";

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
    iss >> port;
    cfg.setPort(port);
}

void DirectiveHandler::_handleServerNameDirective(std::istringstream &iss,
                                                  ServerConfig &cfg) {
    std::string server;
    while (iss >> server) {
        cfg.addServer(server);
    }
}

void DirectiveHandler::_handleErrorPageDirective(std::istringstream &iss,
                                                 ServerConfig &cfg) {
    int errorCode;
    std::string path;
    iss >> errorCode >> path;
    cfg.addErrorPage(errorCode, path);
}

bool DirectiveHandler::_isNumeric(const std::string &str) const {
    return str.find_last_not_of("0123456789") == std::string::npos;
}

bool DirectiveHandler::_isFileReadable(const std::string &path) const {
    struct stat fileInfo;

    return stat(path.c_str(), &fileInfo) &&
           (S_ISREG(fileInfo.st_mode) && fileInfo.st_mode & S_IRUSR);
}

void DirectiveHandler::_handleLocationDirective(std::istringstream &iss,
                                                ServerConfig &cfg) {
    Location location;
    iss >> location._path;
    std::string directive;
    while (iss >> directive) {
        if (directive == "allowed_methods") {
            _handleAllowedMethodsDirective(iss, location);
        }
        if (directive == "index") {
            _handleIndexFiles(iss, location);
        }
    }
    cfg.addLocation(location);
}

void DirectiveHandler::_handleAllowedMethodsDirective(std::istringstream &iss,
                                                      Location &location) {
    std::string httpMethod;
    while (iss >> httpMethod) {
        location._allowedMethods.push_back(httpMethod);
    }
}

void DirectiveHandler::_handleIndexFiles(std::istringstream &iss,
                                         Location &location) {
    std::string file;
    while (iss >> file) {
        location._indexFiles.push_back(file);
    }
}

void DirectiveHandler::process(const std::string &directive,
                               std::istringstream &iss, ServerConfig &cfg) {
    if (_directiveMap.find(directive) != _directiveMap.end()) {
        (this->*_directiveMap[directive])(iss, cfg);
    }
}
