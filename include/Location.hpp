#pragma once

#include <string>
#include <vector>

struct Location {
    std::string path;
    std::vector<std::string> allowedMethods;
    std::string indexFile;
    bool cgi;

    Location() : path(), allowedMethods(), indexFile(), cgi(false) {}

    bool hasPath() const { return !path.empty(); }
    bool hasAllowedMethods() const { return !allowedMethods.empty(); }
    bool hasIndexFile() const { return !indexFile.empty(); }
};
