#ifndef _UTILS_H
#define _UTILS_H

#include <string>
#include <sstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace Utils {


static bool FileExist(const std::string &filepath) {
    struct stat st;
    return stat(filepath.c_str(), &st) == 0;
}

template <typename T>
static std::string toStr(const T &obj) {
    std::stringstream ss;
    ss << obj;
    return ss.str();
}

template <typename T>
static std::string splitArgs(const T &t) {
    return toStr(t);
}

template <typename T, typename ...Args>
static std::string splitArgs(const T &t, Args &&...args) {
    return toStr(t) + splitArgs(std::forward<Args>(args)...);
}


}

#endif // _UTILS_H
