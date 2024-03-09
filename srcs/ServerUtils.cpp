#include "ServerUtils.hpp"

bool ServerUtils::isDirectory(const std::string &path) {
    struct stat info;
    return stat(path.c_str(), &info) == 0 &&
           (S_ISDIR(info.st_mode) && access(path.c_str(), R_OK | X_OK) == 0);
}

bool ServerUtils::isFileReadable(const std::string &path) {
    struct stat fileInfo;
    return stat(path.c_str(), &fileInfo) == 0 &&
           (S_ISREG(fileInfo.st_mode) && fileInfo.st_mode & S_IRUSR);
}

bool ServerUtils::isNumeric(const std::string &str) {
    return str.find_last_not_of("0123456789") == std::string::npos;
}
