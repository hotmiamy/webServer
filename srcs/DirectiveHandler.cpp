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
const std::string DirectiveHandler::ERR_MAX_BODY_SIZE =
    "error at 'client_max_body_size' directive: ";
const std::string DirectiveHandler::ERR_RETURN =
    "error at 'return' directive: ";
const std::string DirectiveHandler::ERR_AUTOINDEX =
    "error at 'autoindex' directive: ";
const std::string DirectiveHandler::ERR_ROOT = "error at 'root' directive: ";
const std::string DirectiveHandler::ERR_CGI = "error at 'cgi' directive: ";

DirectiveHandler::DirectiveHandler() : _cfg() {
    _directiveMap["server_name"] =
        &DirectiveHandler::_handleServerNameDirective;
    _directiveMap["client_max_body_size"] =
        &DirectiveHandler::_handleClientMaxBodySize;
    _directiveMap["listen"] = &DirectiveHandler::_handleListenDirective;
    _directiveMap["error_page"] = &DirectiveHandler::_handleErrorPageDirective;
    _directiveMap["location"] = &DirectiveHandler::_handleLocationDirective;
    _directiveMap["root"] = &DirectiveHandler::_handleRoot;
    _directiveMap["cgi"] = &DirectiveHandler::_handleCgiServer;
}

DirectiveHandler::~DirectiveHandler() {}

ServerConfig DirectiveHandler::getCfg() const { return _cfg; }

void DirectiveHandler::_handleListenDirective(std::istringstream &iss) {
    if (!_cfg.getPort().empty()) {
        throw std::runtime_error(ERR_LISTEN + "'listen' was already specified");
    }

    std::string port;

    if (!(iss >> port) || !ServerUtils::isNumeric(port)) {
        throw std::runtime_error(ERR_LISTEN +
                                 "the port should be a numeric value");
    }
    if (iss.rdbuf()->in_avail() != 0) {
        throw std::runtime_error(
            ERR_LISTEN + "there should be one and only one port per server");
    }
    _cfg.setPort(port);
}

void DirectiveHandler::_handleServerNameDirective(std::istringstream &iss) {
    if (!_cfg.getServerName().empty()) {
        throw std::runtime_error(ERR_SERVER_NAME +
                                 "'server_name' was already specified");
    }

    std::string server;
    if (!(iss >> server)) {
        throw std::runtime_error(ERR_SERVER_NAME +
                                 "a server name should be provided");
    }

    if (iss.rdbuf()->in_avail() != 0) {
        throw std::runtime_error(ERR_SERVER_NAME +
                                 "this directive takes only one argument");
    }
    _cfg.setServer(server);
}

void DirectiveHandler::_handleErrorPageDirective(std::istringstream &iss) {
    if (_cfg.getRoot().empty()) {
        throw std::runtime_error(
            ERR_ERROR_PAGE +
            "make sure 'root' is set before the 'location' blocks");
    }

    std::string errorCode, path;

    if (!(iss >> errorCode) || !ServerUtils::isNumeric(errorCode)) {
        throw std::runtime_error(ERR_ERROR_PAGE +
                                 "the error code should be a numeric value");
    }

    if (!(iss >> path) ||
        !ServerUtils::isFileReadable(_cfg.getRoot() + "/" + path)) {
        throw std::runtime_error(ERR_ERROR_PAGE +
                                 "page should point to a valid path");
    }
    if (iss.rdbuf()->in_avail() != 0) {
        throw std::runtime_error(
            ERR_ERROR_PAGE +
            "there shouldn't exist any more args for this directive, just "
            "<error_code> <error_page>");
    }
    std::string fullPath = _cfg.getRoot() + "/" + path;
    _cfg.addErrorPage(errorCode, fullPath);
}

void DirectiveHandler::_handleLocationDirective(std::istringstream &iss) {
    if (_cfg.getRoot().empty()) {
        throw std::runtime_error(
            ERR_LOCATION +
            "make sure 'root' is set before the 'location' blocks");
    }

    Location location;
    std::string line;
    while (std::getline(iss, line)) {
        std::istringstream lineIss(line);
        if (!location.hasPath()) {
            _resolvePath(lineIss, location);
            continue;
        }
        if (line.find("autoindex") != std::string::npos) {
            _handleAutoindex(lineIss, location);
            continue;
        }
        if (line.find("index") != std::string::npos) {
            _resolveIndexFile(lineIss, location);
            continue;
        }
        if (line.find("allowed_methods") != std::string::npos) {
            _resolveAllowedMethods(lineIss, location);
            continue;
        }
        if (line.find("cgi") != std::string::npos) {
            _handleCgi(lineIss, location);
            continue;
        }
        if (line.find("return") != std::string::npos) {
            _handleReturn(lineIss, location);
        }
    }
    _cfg.addLocation(location.path, location);
}

void DirectiveHandler::_resolvePath(std::istringstream &lineIss,
                                    Location &location) {
    if (!(lineIss >> location.path) || location.path == "{") {
        throw std::runtime_error(ERR_LOCATION + "a path must be specified");
    }
    std::string tmp;
    if (!(lineIss >> tmp) || tmp != "{") {
        throw std::runtime_error(ERR_LOCATION + "no '{' found");
    }
    if (!ServerUtils::isDirectory(_cfg.getRoot() + location.path)) {
        throw std::runtime_error(ERR_LOCATION + "'" + location.path + "'" +
                                 " is not a valid directory");
    }
}

void DirectiveHandler::_resolveIndexFile(std::istringstream &lineIss,
                                         Location &location) {
    if (location.hasIndexFiles()) {
        throw std::runtime_error(ERR_LOCATION +
                                 "index file was already defined");
    }
    lineIss.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
    _handleIndexFile(lineIss, location);
}

void DirectiveHandler::_resolveAllowedMethods(std::istringstream &lineIss,
                                              Location &location) {
    if (location.hasAllowedMethods()) {
        throw std::runtime_error(ERR_LOCATION +
                                 "allowed methods are already defined");
    }
    lineIss.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
    _handleAllowedMethodsDirective(lineIss, location);
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
        if (std::find(location.allowedMethods.begin(),
                      location.allowedMethods.end(),
                      httpMethod) != location.allowedMethods.end()) {
            throw std::runtime_error(ERR_ALLOWED_METHODS + httpMethod +
                                     " is already included");
        }
        location.allowedMethods.push_back(httpMethod);
    }
}

bool DirectiveHandler::_isAllowedHttpMethod(const std::string &str) const {
    return str == "GET" || str == "POST" || str == "DELETE";
}

void DirectiveHandler::_handleIndexFile(std::istringstream &iss,
                                        Location &location) {
    std::string path = _cfg.getRoot() + location.path;
    std::string fileName;

    if (path.at(path.size() - 1) != '/') {
        path += "/";
    }
    if (!(iss >> fileName) || !ServerUtils::isFileReadable(path + fileName)) {
        throw std::runtime_error(ERR_LOCATION + "no such file '" + fileName +
                                 "' at " + path);
    }
    location.indexFile = path + fileName;
    if (iss.rdbuf()->in_avail() != 0) {
        throw std::runtime_error(ERR_LOCATION +
                                 "'index' directive takes only one argument");
    }
}

void DirectiveHandler::_handleRoot(std::istringstream &iss) {
    if (!_cfg.getRoot().empty()) {
        throw std::runtime_error(ERR_ROOT + "root was already specified");
    }

    std::string path;
    if (!(iss >> path) || !ServerUtils::isDirectory(path)) {
        throw std::runtime_error(ERR_ROOT +
                                 "a valid directory must be specified");
    }
    if (iss.rdbuf()->in_avail() != 0) {
        throw std::runtime_error(
            ERR_ROOT + "there should be one and only one directory specified");
    }
    _cfg.setRoot(path);
}

void DirectiveHandler::_handleCgi(std::istringstream &iss, Location &location) {
    std::string aux, extension, executable;
    iss >> aux;

    if (location.cgi) {
        throw std::runtime_error(ERR_CGI + "cgi was already specified");
    }

    if (!(iss >> extension) || extension != ".py") {
        throw std::runtime_error(
            ERR_CGI + "a valid file extension (.py) must be specified");
    }
    if (!(iss >> executable) || !ServerUtils::isValidExecutable(executable)) {
        throw std::runtime_error(ERR_CGI + '\'' + executable + '\'' +
                                 " is not a valid binary");
    }
    if (iss.rdbuf()->in_avail() != 0) {
        throw std::runtime_error(
            ERR_CGI + "there should be one and only one executable specified");
    }
    location.cgi = true;
}

void DirectiveHandler::_handleReturn(std::istringstream &iss,
                                     Location &location) {
    std::string aux, code, redirect;
    iss >> aux;

    if (location.redirection.second != "") {
        throw std::runtime_error(ERR_RETURN + "already defined");
    }
    if (!(iss >> code) || !ServerUtils::withinRange(ServerUtils::stoi(code),
                                                    std::make_pair(300, 399))) {
        throw std::runtime_error(
            ERR_RETURN + "first argument should be a numeric value (30x)");
    }
    if (!(iss >> redirect)) {
        throw std::runtime_error(ERR_RETURN + "an URL should be specified");
    }
    if (iss.rdbuf()->in_avail() != 0) {
        throw std::runtime_error("this directive takes only two arguments");
    }
    std::stringstream ss;
    ss << code;
    location.redirection = std::make_pair(ss.str(), redirect);
}

void DirectiveHandler::_handleAutoindex(std::istringstream &iss,
                                        Location &location) {
    if (location.autoindex) {
        throw std::runtime_error(ERR_AUTOINDEX + "already defined");
    }

    std::string aux, flag;
    iss >> aux;
    if (!(iss >> flag) || flag != "on") {
        throw std::runtime_error(ERR_AUTOINDEX +
                                 "argument should be a flag ('on')");
    }
    if (iss.rdbuf()->in_avail() != 0) {
        throw std::runtime_error(ERR_AUTOINDEX +
                                 "this directive takes only one argument");
    }
    location.autoindex = true;
}

void DirectiveHandler::_handleClientMaxBodySize(std::istringstream &iss) {
    if (!_cfg.getClientMaxBodySize().empty()) {
        throw std::runtime_error(ERR_MAX_BODY_SIZE +
                                 "directive was already defined");
    }

    std::string maxBodySize;
    if (!(iss >> maxBodySize) || !ServerUtils::isNumeric(maxBodySize)) {
        throw std::runtime_error(
            ERR_MAX_BODY_SIZE +
            "the argument should be a numeric value\ngot instead: " +
            maxBodySize);
    }
    if (iss.rdbuf()->in_avail() != 0) {
        throw std::runtime_error(ERR_MAX_BODY_SIZE +
                                 "this directive takes only one argument");
    }
    _cfg.setClientMaxBodySize(maxBodySize);
}

void DirectiveHandler::_handleCgiServer(std::istringstream &iss) {
    std::string extension, executable;

    if (_cfg.cgi) {
        throw std::runtime_error(ERR_CGI + "cgi was already specified");
    }

    if (!(iss >> extension) || extension != ".py") {
        throw std::runtime_error(
            ERR_CGI + "a valid file extension (.py) must be specified");
    }
    if (!(iss >> executable) || !ServerUtils::isValidExecutable(executable)) {
        throw std::runtime_error(ERR_CGI + '\'' + executable + '\'' +
                                 " is not a valid binary");
    }
    if (iss.rdbuf()->in_avail() != 0) {
        throw std::runtime_error(
            ERR_CGI + "there should be one and only one executable specified");
    }
    _cfg.cgi = true;
}

void DirectiveHandler::process(const std::string &directive,
                               std::istringstream &iss) {
    if (_directiveMap.find(directive) != _directiveMap.end()) {
        return (this->*_directiveMap[directive])(iss);
    }

    if (directive != "}" && directive != "server") {
        throw std::runtime_error("invalid directive: " + directive);
    }
}
