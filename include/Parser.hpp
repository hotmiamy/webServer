#pragma once

#include <sstream>

#include "ServerConfig.hpp"

class Parser {
   public:
    Parser();
    Parser(const Parser &);
    Parser &operator=(const Parser &);
    ~Parser();

    static std::vector<ServerConfig> parse(std::ifstream &);

   private:
};
