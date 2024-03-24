#pragma once

#include <unistd.h>
#include <wait.h>

#include <cstdio>
#include <vector>

#include "ReqParsing.hpp"

class Cgi {
   public:
    Cgi(const Cgi &);
    Cgi(ReqParsing &);
    Cgi &operator=(const Cgi &);
    ~Cgi();

    void execute();

    const std::string &getOut() const;

   private:
    const ReqParsing _request;
    std::string _script;
    std::string _binaryAbsPath;
    int _pipedes[2];
    std::string _out;

    void _setup();
    void _childRoutine();
    void _parentRoutine(ssize_t);
    std::vector<std::string> _setupEnv();
};
