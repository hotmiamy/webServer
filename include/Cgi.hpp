#pragma once

#include <unistd.h>
#include <wait.h>

#include <cstdio>
#include <cstring>
#include <vector>

#include "ReqParsing.hpp"

class Cgi {
   public:
    Cgi(const Cgi &);
    Cgi(const ReqParsing &, const std::string &);
    Cgi &operator=(const Cgi &);
    ~Cgi();

    void execute();

    const std::string &getOut() const;

   private:
    ReqParsing _request;
    std::string _script;
    std::string _binaryAbsPath;
    int _pipedes[2];
    std::string _out;
    std::string _httpMethod;

    void _setup();
    void _childRoutine();
    void _parentRoutine(ssize_t);
    std::vector<std::string> _setupEnv();
};
