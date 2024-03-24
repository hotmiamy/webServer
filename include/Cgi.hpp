#pragma once

#include <sched.h>
#include <unistd.h>

#include <cstdio>
#include <vector>

#include "ReqParsing.hpp"

namespace Cgi {

bool isDefined(ReqParsing &);
void execute(ReqParsing &);
void childRoutine(ReqParsing &);
std::vector<std::string> setupEnv(ReqParsing &);

}  // namespace Cgi
