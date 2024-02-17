#pragma once

#include <sys/stat.h>
#include <unistd.h>
#include <iostream>

template <typename T>
bool isDirectory(const T &path)
{
	struct stat info;
	return stat(path.c_str(), &info) == 0 &&
           (S_ISDIR(info.st_mode) && access(path.c_str(), R_OK | X_OK) == 0);
}
