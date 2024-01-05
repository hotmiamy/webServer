#include <iostream>
#include <vector>

#include "ServerConfig.hpp"
#include "WebServer.hpp"

int run(const std::vector<ServerConfig> &configs) {
    std::vector<WebServer> servers(configs.size());

    for (;;) {
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "usage: \'./webserv [configuration file]\'\n";
        return 1;
    }

    return run(ServerConfig::fromFile(argv[1]));
}
