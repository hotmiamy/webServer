#include "Parser.hpp"

Parser::Parser() {}

Parser::Parser(const Parser &other) { *this = other; }

Parser &Parser::operator=(const Parser &other) {
    if (this != &other) {
    }
    return *this;
}

Parser::~Parser() {}

std::vector<ServerConfig> Parser::parse(std::ifstream &ifs) {
    std::stringstream ss;
    ss << ifs.rdbuf();
    std::string fileContent = ss.str();

    std::vector<ServerConfig> configs;
    return configs;
}
