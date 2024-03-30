#include <iostream>

#include "ServerConfig.hpp"
#include "WebServer.hpp"

int main(int argc, char *argv[]) {    if (argc != 2) {
        std::cerr << "usage: \'./webserv [configuration file]\'\n";
        return 1;
    }
    try {
        WebServer webServer;
        webServer.init(argv);
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
