#pragma once

#include <map>
#include <vector>
#include <string>
#include <ctime> 
#include "Location.hpp"

namespace ReqParsUtils {

bool IsMethodAllowed(const Location &locat, std::string method);

std::string ExtractHeader(std::string const &request, std::string key);

std::string ContentFormat(std::string key);

std::string StatusCodes(std::string code);

std::string getCurrDate(void);

bool hasBody(const std::string &request);
	
}