#pragma once

#include <string>
#include <vector>

struct Location {
    std::string path;
    std::vector<std::string> allowedMethods;
    std::vector<std::string> indexFiles;

    Location() : path(), allowedMethods(), indexFiles() {}

    bool pathSet() const { return !path.empty(); }
    bool allowedMethodsSet() const { return !allowedMethods.empty(); }
    bool indexFilesSet() const { return !indexFiles.empty(); }
};
