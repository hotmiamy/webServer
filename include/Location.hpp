#pragma once

#include <string>
#include <vector>

struct Location {
    std::string _path;
    std::vector<std::string> _allowedMethods;
};
