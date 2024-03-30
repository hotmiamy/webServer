#pragma once

#include <string>
#include <vector>

struct Location {
    std::string path;
    std::vector<std::string> allowedMethods;
    std::string indexFile;
    bool cgi;
    std::pair<std::string, std::string> redirection;
    bool autoindex;

    Location()
        : path(),
          allowedMethods(),
          indexFile(),
          cgi(false),
          redirection(),
          autoindex(false) {}

    bool hasPath() const { return !path.empty(); }
    bool hasAllowedMethods() const { return !allowedMethods.empty(); }
    bool hasIndexFiles() const { return !indexFile.empty(); }
    bool empty() const {
        return path.empty() && allowedMethods.empty() && indexFile.empty();
    }
    bool redirectionSet() const {
        return !(redirection.first.empty() || redirection.second.empty());
    }
};
