#pragma once

#include <sys/stat.h>
#include <unistd.h>

#include <string>

namespace ServerUtils {

bool isDirectory(const std::string &path);

bool isFileReadable(const std::string &path);

bool isNumeric(const std::string &str);

}  // namespace ServerUtils
