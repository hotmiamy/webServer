#include <iostream>

#include "ServerConfig.hpp"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "usage: \'./webserv [configuration file]\'\n";
        return 1;
    }

    ServerConfig config = ServerConfig::fromFile(argv[1]);
    return 0;
}
