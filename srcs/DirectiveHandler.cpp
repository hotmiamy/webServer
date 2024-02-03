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
const std::string DirectiveHandler::ERR_ROOT = "error at 'root' directive: ";

DirectiveHandler::DirectiveHandler() : _cfg() {
    _directiveMap["listen"] = &DirectiveHandler::_handleListenDirective;
    _directiveMap["server_name"] =
        &DirectiveHandler::_handleServerNameDirective;
    _directiveMap["error_page"] = &DirectiveHandler::_handleErrorPageDirective;
    _directiveMap["location"] = &DirectiveHandler::_handleLocationDirective;
    _directiveMap["root"] = &DirectiveHandler::_handleRoot;
}

DirectiveHandler::~DirectiveHandler() {}

ServerConfig DirectiveHandler::getCfg() const { return _cfg; }

void DirectiveHandler::_handleListenDirective(std::istringstream &iss) {
    if (!_cfg.getPort().empty()) {
        throw std::runtime_error(ERR_LISTEN + "'listen' was already specified");
    }

    std::string port;

    if (!(iss >> port) || !_isNumeric(port)) {
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
    if (!_cfg.getServerNames().empty()) {
        throw std::runtime_error(ERR_SERVER_NAME +
                                 "'server_name' was already specified");
    }

    std::string server;

    if (!(iss >> server)) {
        throw std::runtime_error(ERR_SERVER_NAME +
                                 "at least one server name should be provided");
    }
    _cfg.addServer(server);
    while (iss >> server) {
        _cfg.addServer(server);
    }
}

void DirectiveHandler::_handleErrorPageDirective(std::istringstream &iss) {
    if (!_cfg.getErrorPages().empty()) {
        throw std::runtime_error(ERR_ERROR_PAGE +
                                 "'error_page' was already specified");
    }

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

    _cfg.addErrorPage(errorCode, path);
}

bool DirectiveHandler::_isNumeric(const std::string &str) const {
    return str.find_last_not_of("0123456789") == std::string::npos;
}

bool DirectiveHandler::_isFileReadable(const std::string &path) const {
    struct stat fileInfo;
    return stat(path.c_str(), &fileInfo) == 0 &&
           (S_ISREG(fileInfo.st_mode) && fileInfo.st_mode & S_IRUSR);
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
        }
        if (line.find("index") != std::string::npos) {
            _resolveIndexFiles(lineIss, location);
        }
        if (line.find("allowed_methods") != std::string::npos) {
            _resolveAllowedMethods(lineIss, location);
        }
    }
    _cfg.addLocation(location);
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
    if (!_isDirectory(_cfg.getRoot() + location.path)) {
        throw std::runtime_error(ERR_LOCATION + "'" + location.path + "'" +
                                 " is not a valid directory");
    }
}

void DirectiveHandler::_resolveIndexFiles(std::istringstream &lineIss,
                                          Location &location) {
    if (location.hasIndexFiles()) {
        throw std::runtime_error(ERR_LOCATION +
                                 "index files are already defined");
    }
    lineIss.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
    _handleIndexFiles(lineIss, location);
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
    return str == "GET" || str == "POST";
}

void DirectiveHandler::_handleIndexFiles(std::istringstream &iss,
                                         Location &location) {
    std::string path = _cfg.getRoot() + location.path;
    std::string file;

    if (!(iss >> file) || !_isFileReadable(path + file)) {
        throw std::runtime_error(ERR_LOCATION + "no such file '" + file +
                                 "' at " + path);
    }
    while (iss >> file) {
        if (!_isFileReadable(path + file)) {
            throw std::runtime_error(ERR_LOCATION + "no such file '" + file +
                                     "' at " + path);
        }
        location.indexFiles.push_back(file);
    }
}

void DirectiveHandler::_handleRoot(std::istringstream &iss) {
    if (!_cfg.getRoot().empty()) {
        throw std::runtime_error(ERR_ROOT + "root was already specified");
    }

    std::string path;
    if (!(iss >> path) || !_isDirectory(path)) {
        throw std::runtime_error(ERR_ROOT +
                                 "a valid directory must be specified");
    }
    if (iss.rdbuf()->in_avail() != 0) {
        throw std::runtime_error(
            ERR_ROOT + "there should be one and only one directory specified");
    }
    _cfg.setRoot(path);
}

bool DirectiveHandler::_isDirectory(const std::string &path) const {
    struct stat info;
    return stat(path.c_str(), &info) == 0 &&
           (S_ISDIR(info.st_mode) && access(path.c_str(), R_OK | X_OK) == 0);
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
