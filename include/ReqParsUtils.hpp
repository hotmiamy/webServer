#pragma once

#include <map>
#include <vector>
#include <string>
#include "Location.hpp"
#include "iostream"

namespace ReqParsUtils {

std::string ExtractHeader(std::string const &request, std::string key);

std::string ContentFormat(std::string key);
	
void trimHostname(std::string &hostname);

}