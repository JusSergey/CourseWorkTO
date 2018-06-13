#ifndef SUBSYSTEM_H
#define SUBSYSTEM_H

#include <sys/sysinfo.h>
#include "../Utils/StringUtilities.h"
#include "types.h"
#include <cstdlib>

class ActiveDev {
public:
    using TestResult = std::pair<std::string, double>;
    virtual TestResult test() = 0;
};

class Subsystem
{
    DEVICE dev;
public:
    Subsystem(DEVICE devType);
    static struct sysinfo getGlobalInfo();

    virtual ListStrVects getDeviceInfo() const { return getStructureInfo(getPrintableInfo()); }
    virtual string getPrintableInfo() const = 0;
    virtual string getHTMLCode() const;
    string getPrintableInfo(const ListStrVects &src, const StringSet &filter) const;

    ListStrVects getStructureInfo(const string &src) const;

protected:
    static string _getDataFromFile(const std::string &pathToFile);
    static string _getDataFromCommand(const std::string &command);
    static string _alignText(const std::string &str);
    static double _getItervalBetweenCalculate(const std::function<void()> &func);
};

#endif // SUBSYSTEM_H
