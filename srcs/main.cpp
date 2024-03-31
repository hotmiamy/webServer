#include <iostream>

#include "ServerConfig.hpp"
#include "WebServer.hpp"

WebServer webServer;

void interrupt(int sig) {
    std::cout << '\n';
    std::cout << "Received signal " << (sig + 128) << '\n';
    webServer.stop();
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "usage: \'./webserv [configuration file]\'\n";
        return 1;
    }

    struct sigaction interruptHandler;
    interruptHandler.sa_handler = interrupt;
    sigemptyset(&interruptHandler.sa_mask);
    interruptHandler.sa_flags = 0;
    sigaction(SIGINT, &interruptHandler, 0);

    try {
        webServer = WebServer(ServerConfig::fromFile(argv[1]));
        webServer.init();
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
