#pragma once

#include <string>
#include <vector>

struct Location {
    std::string path;
    std::vector<std::string> allowedMethods;
    std::string indexFile;
    bool cgi;
    std::pair<std::string, std::string> redirection;

    Location()
        : path(), allowedMethods(), indexFile(), cgi(false), redirection() {}

    bool hasPath() const { return !path.empty(); }
    bool hasAllowedMethods() const { return !allowedMethods.empty(); }
    bool hasIndexFiles() const { return !indexFile.empty(); }
    bool empty() const {
        return path.empty() && allowedMethods.empty() && indexFile.empty();
    }
};
