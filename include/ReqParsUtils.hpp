#pragma once

#include <map>
#include <vector>
#include <string>
#include "Location.hpp"

namespace ReqParsUtils {

bool IsMethodAllowed(const Location &locat, std::string method);

std::string ExtractHeader(std::string const &request, std::string key);

std::string ContentFormat(std::string key);

std::string StatusCodes(std::string code);
	
}