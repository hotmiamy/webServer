#include "Cgi.hpp"

namespace Cgi {

bool isDefined(ReqParsing &r) {
    if (!r.getLocation()) {
        return false;
    }
    return r.getLocation()->cgi;
}

void execute(ReqParsing &r) {
    std::cout << "executando o cgi...\n";

    std::string script = "server_root" + r.getUrl();
    std::cout << "script: " << script << '\n';
    std::string binaryAbsPath = ServerUtils::getAbsPath("python");
    std::cout << "binaryAbsPath:" << binaryAbsPath << '\n';

    int pipedes[2];
    if (pipe(pipedes) < 0) {
        std::perror("cgi pipe");
        return;
    }

    pid_t pid = fork();
    if (-1 == pid) {
        perror("cgi fork");
        return;
    }

    if (0 == pid) {
        childRoutine(r);
    } else {
    }
}

void childRoutine(ReqParsing &r) {
    std::vector<std::string> envpVec = setupEnv(r);
}

std::vector<std::string> setupEnv(ReqParsing &r) {
    std::vector<std::string> envpVec;
    envpVec.push_back("SERVER_PROTOCOL=HTTP/1.1");
    envpVec.push_back("GATEWAY_INTERFACE=CGI/1.1");
    envpVec.push_back("SERVER_SOFTWARE=webserv");
    // todo: change these
    envpVec.push_back("REQUEST_METHOD=GET");
    envpVec.push_back("SERVER_NAME=localhost");
    envpVec.push_back("SERVER_PORT=8080");
    envpVec.push_back("CONTENT_TYPE=text/html");
    std::stringstream len;
    len << r.getBody().length();
    envpVec.push_back("CONTENT_LENGTH=" + len.str());
    envpVec.push_back("SCRIPT_NAME=");
    return envpVec;
}

}  // namespace Cgi
