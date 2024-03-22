#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include "Location.hpp"

namespace ResponseUtils {
	std::string StatusCodes(std::string code);
	bool IsMethodAllowed(const Location &locat, std::string method);
}