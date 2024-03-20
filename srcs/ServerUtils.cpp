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

bool checkFileExist(const std::string &file)
{
	struct stat fileInfo;
	if (stat(file.c_str(), &fileInfo) < 0)
		return false;
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

}  // namespace ServerUtils
