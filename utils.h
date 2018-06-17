#ifndef _UTILS_H
#define _UTILS_H

#include <string>
#include <sstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <QtWidgets>
#include "SysInfo/Subsystem.h"
#include <types.h>
#include "Utils/NetUtils.h"

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

static std::string generateFilename() {

    return Utils::splitArgs(NetUtils::getLocalIP(), '@', NetUtils::getHostName(), '@',
                            QDate::currentDate().toString(Qt::SystemLocaleShortDate).toStdString(), '@',
                            QTime::currentTime().toString(Qt::ISODateWithMs).toStdString(), ".html");
}


}

#endif // _UTILS_H
