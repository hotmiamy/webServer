#pragma once

#include <map>
#include <vector>
#include <string>
#include "Location.hpp"
#include "iostream"

namespace ReqParsUtils {

std::string ExtractHeader(std::string const &request, std::string key);

std::string ContentFormat(std::string key);

void extractBody(const std::string &request, std::string &body, std::string &form);
	
}