#pragma once

#include <map>
#include <vector>
#include <string>
#include "Location.hpp"

namespace ReqParsUtils {

std::string ExtractHeader(std::string const &request, std::string key);

std::string ContentFormat(std::string key);

bool hasBody(const std::string &request);
	
}