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
    system("ifconfig | grep 'inet addr' > /tmp/_cw_ifconfig.output");
    string dataFromFile = Subsystem::_getDataFromCommand("ifconfig | grep 'inet addr'");
    string ip;
    for (auto str : StringUtil::cropToStrings(dataFromFile, '\n')) {
        auto lines = StringUtil::cropToStrings(str, ' ');
        if (lines.size() == 4) {
            auto linesWithIp = StringUtil::cropToStrings(StringUtil::strListToStrVec(lines)[2], ':');
            if (linesWithIp.size() == 2){
                ip = linesWithIp.back();
                break;
            }
        }
    }

    char buffer[128]{0};
    gethostname(buffer, sizeof(buffer)-1);

    return Utils::splitArgs(ip, '@', buffer, '@',
                                         QDate::currentDate().toString(Qt::SystemLocaleShortDate).toStdString(), '@',
                                         QTime::currentTime().toString(Qt::ISODateWithMs).toStdString(), ".html");
}

}

#endif // _UTILS_H
