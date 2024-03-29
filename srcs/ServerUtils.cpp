#include "ServerUtils.hpp"

namespace ServerUtils {

bool isDirectory(const std::string &path) {
    struct stat info;
    return stat(path.c_str(), &info) == 0 &&
           (S_ISDIR(info.st_mode) && access(path.c_str(), R_OK | X_OK) == 0);
}

bool isFileReadable(const std::string &path) {
    struct stat fileInfo;
    return stat(path.c_str(), &fileInfo) == 0 &&
           (S_ISREG(fileInfo.st_mode) && fileInfo.st_mode & S_IRUSR);
}

bool fileExists(const std::string &file) {
    struct stat fileInfo;
    if (stat(file.c_str(), &fileInfo) < 0) return false;
    return true;
}

bool isNumeric(const std::string &str) {
    return str.find_last_not_of("0123456789") == std::string::npos;
}

const std::string getExtension(const std::string &path) {
    size_t pos = path.find_last_of('.');
    if (pos != std::string::npos) {
        return path.substr(pos + 1);
    }
    return "";
}

bool isValidExecutable(const std::string &ex) {
    const char *pathEnv = getenv("PATH");
    if (!pathEnv) {
        return false;
    }

    std::istringstream iss(pathEnv);
    std::string dir;
    while (std::getline(iss, dir, ':')) {
        std::string path = dir + '/' + ex;
        if (access(path.c_str(), X_OK) == 0) {
            return true;
        }
    }
    return false;
}

const std::string getAbsPath(const std::string &ex) {
    const char *pathEnv = getenv("PATH");
    if (!pathEnv) {
        return "";
    }

    std::istringstream iss(pathEnv);
    std::string dir;
    while (std::getline(iss, dir, ':')) {
        std::string path = dir + '/' + ex;
        if (access(path.c_str(), X_OK) == 0) {
            return path;
        }
    }
    return "";
}

std::vector<std::pair<std::string, std::string> > getDefaultErrorPages() {
    std::vector<std::pair<std::string, std::string> > defaultErrorPages;

    defaultErrorPages.push_back(
        std::make_pair("404", "./server_root/error_pages/error404.html"));

    for (std::vector<std::pair<std::string, std::string> >::const_iterator it =
             defaultErrorPages.begin();
         it != defaultErrorPages.end(); ++it) {
        if (!isFileReadable(it->second)) {
            throw std::runtime_error("no such file: " + it->second);
        }
    }

    return defaultErrorPages;
}

}  // namespace ServerUtils
