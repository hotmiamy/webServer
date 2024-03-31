#pragma once

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdlib>
#include <map>
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

std::vector<std::pair<std::string, std::string> > getDefaultErrorPages(
    const std::map<std::string, std::string> &);

bool withinRange(int n, std::pair<int, int> range);

int stoi(const std::string &str);

}  // namespace ServerUtils
