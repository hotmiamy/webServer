#pragma once

#include <string>
#include <vector>

struct Location {
    std::string path;
    std::vector<std::string> allowedMethods;
    std::vector<std::string> indexFiles;
    bool cgi;

    Location() : path(), allowedMethods(), indexFiles(), cgi(false) {}

    bool hasPath() const { return !path.empty(); }
    bool hasAllowedMethods() const { return !allowedMethods.empty(); }
    bool hasIndexFiles() const { return !indexFiles.empty(); }
};
