#include "Cgi.hpp"

Cgi::Cgi(const Cgi &rhs) { *this = rhs; }

Cgi::Cgi(ReqParsing &request, const std::string &httpMethod)
    : _request(request),
      _script(),
      _binaryAbsPath(),
      _pipedes(),
      _out(),
      _httpMethod(httpMethod) {}

Cgi &Cgi::operator=(const Cgi &other) {
    if (this != &other) {
    }
    return *this;
}

Cgi::~Cgi() {}

void Cgi::_setup() {
    _script = "./server_root" + _request.getUrl();
    _binaryAbsPath = ServerUtils::getAbsPath("python3");
}

void Cgi::execute() {
    _setup();

    if (pipe(_pipedes) < 0) {
        return std::perror("pipe (cgi)");
    }
    dup2(_pipedes[0], STDIN_FILENO);
    dup2(_pipedes[1], STDOUT_FILENO);

    if (_httpMethod == "POST") {
        write(_pipedes[1], _request.getBody().c_str(),
              _request.getBody().length());
    }

    pid_t pid = fork();
    if (-1 == pid) {
        return std::perror("fork (cgi)");
    }
    if (0 == pid) {
        _childRoutine();
    }
    _parentRoutine(pid);
}

void Cgi::_childRoutine() {
    const char *argv[] = {_binaryAbsPath.c_str(), _script.c_str(), NULL};
    std::vector<std::string> envpVec = _setupEnv();
    const char *envp[] = {
        envpVec[0].c_str(), envpVec[1].c_str(),  envpVec[2].c_str(),
        envpVec[3].c_str(), envpVec[4].c_str(),  envpVec[5].c_str(),
        envpVec[6].c_str(), envpVec[7].c_str(),  envpVec[8].c_str(),
        envpVec[9].c_str(), envpVec[10].c_str(), NULL};

    if (execve(argv[0], (char *const *)argv, (char *const *)envp) < 0) {
        std::perror("execve (cgi)");
    }
}

void Cgi::_parentRoutine(ssize_t pid) {
    waitpid(pid, NULL, 0);
    // waitpid(pid, NULL, WNOHANG);

    char buf[BUFSIZ] = {0};

    ssize_t ret = read(_pipedes[0], buf, BUFSIZ);
    if (-1 == ret) {
        perror("read (cgi)");
    } else {
        _out += buf;
    }

    close(_pipedes[0]);
    close(_pipedes[1]);
}

std::vector<std::string> Cgi::_setupEnv() {
    std::vector<std::string> envpVec;

    envpVec.push_back("SERVER_PROTOCOL=HTTP/1.1");
    envpVec.push_back("GATEWAY_INTERFACE=CGI/1.1");
    envpVec.push_back("SERVER_SOFTWARE=webserv");
    envpVec.push_back("REQUEST_METHOD=" + _httpMethod);
    envpVec.push_back("SERVER_NAME=localhost");
    envpVec.push_back("SERVER_PORT=8080");
    envpVec.push_back("REQUEST_BODY=" + _request.getBody());
    if (_httpMethod == "POST") {
        envpVec.push_back("CONTENT_TYPE=" + _request.getContentType());
    } else {
        envpVec.push_back("CONTENT_TYPE=text/html");
    }
    envpVec.push_back("SCRIPT_NAME=" + _script);
    std::stringstream ss;
    ss << _request.getBody().length();
    envpVec.push_back("CONTENT_LENGTH=" + ss.str());
    envpVec.push_back("PATH_INFO=" + _script);

    return envpVec;
}

const std::string &Cgi::getOut() const { return _out; }
