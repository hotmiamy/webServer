#include <iostream>

#include "ServerConfig.hpp"
#include "WebServer.hpp"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "usage: \'./webserv [configuration file]\'\n";
        return 1;
    }

    ServerConfig config = ServerConfig::fromFile(argv[1]);
    WebServer webServer(config);
    // return webServer.run();
    return 0;
}
