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

    std::string errorPagesDir = "./server_root/error_pages/";

    DIR *dir = opendir(errorPagesDir.c_str());
    if (!dir) {
        throw std::runtime_error("error opening " + errorPagesDir);
    }

    dirent *entry;
    while ((entry = readdir(dir))) {
        std::string fileName = entry->d_name;
        if (ServerUtils::getExtension(fileName) == "html") {
            std::string errorCode = fileName.substr(0, fileName.length() - 5),
                        filePath = errorPagesDir + fileName;
            if (!isFileReadable(filePath)) {
                throw std::runtime_error("no such file: " + filePath);
            }
            defaultErrorPages.push_back(std::make_pair(errorCode, filePath));
        }
    }
    closedir(dir);

    return defaultErrorPages;
}

bool withinRange(int n, std::pair<int, int> range) {
    return n >= range.first && n <= range.second;
}

int stoi(const std::string &str) {
    std::istringstream iss(str);
    int res;
    iss >> res;
    return res;
}

}  // namespace ServerUtils
