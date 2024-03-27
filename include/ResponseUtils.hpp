#include <ctime>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "Location.hpp"
#include "ReqParsing.hpp"

namespace ResponseUtils {

std::string StatusCodes(const std::string &code);
bool IsMethodAllowed(const Location &locat, const std::string &method);
std::string getCurrDate(void);
std::string genFileName(ReqParsing &req);

}  // namespace ResponseUtils
