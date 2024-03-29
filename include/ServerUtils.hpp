#pragma once

#include <sys/stat.h>
#include <unistd.h>

#include <cstdlib>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace ServerUtils {

bool isDirectory(const std::string &path);

bool isFileReadable(const std::string &path);

bool isNumeric(const std::string &str);

const std::string getExtension(const std::string &path);

bool fileExists(const std::string &file);

bool isValidExecutable(const std::string &ex);

const std::string getAbsPath(const std::string &ex);

std::vector<std::pair<std::string, std::string> > getDefaultErrorPages();

}  // namespace ServerUtils
