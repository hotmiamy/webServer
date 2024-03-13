#pragma once

#include <sys/stat.h>
#include <unistd.h>

#include <string>
#include "ReqParsUtils.hpp"

namespace ServerUtils {

bool isDirectory(const std::string &path);

bool isFileReadable(const std::string &path);

bool isNumeric(const std::string &str);

const std::string getExtension(const std::string &path);

bool checkFileExist(const std::string &file);

}  // namespace ServerUtils
