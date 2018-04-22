#ifndef SUBSYSTEM_H
#define SUBSYSTEM_H

#include <sys/sysinfo.h>
#include "StringUtilities.h"
#include <iostream>
#include <string>
#include <list>
#include <set>
#include <cstdlib>
using std::string;
using std::pair;
using std::list;
using std::set;

using PairType = pair<string, string>;

struct InfoPair : public PairType
{
    PairType::first_type  &a;
    PairType::second_type &b;

    InfoPair() : a(first), b(second) {}
};

using ListInfoT = list<PairType>;

using StringSet = set<string>;

class ActiveDev {
public:
    using TestResult = std::pair<std::string, double>;
    virtual TestResult test() = 0;
};

class NoActiveDev {
public:
    virtual ListInfoT getDeviceInfo() const = 0;
};

class Subsystem
{
public:
    Subsystem();
//    virtual ListInfoT getDeviceInfo() = 0;
//    virtual double test() = 0;
    struct sysinfo getGlobalInfo();

    virtual string getPrintableInfo() const = 0;
    string getPrintableInfo(const ListInfoT &src, const StringSet &filter) const;
    ListInfoT getStructureInfo(const string &src) const;

protected:
    static string _getDataFromFile(const std::string &pathToFile);
    static string _getDataFromCommand(const std::string &command);
    static string _alignText(const std::string &str);
    static double _getItervalBetweenCalculate(const std::function<void()> &func);
};

#endif // SUBSYSTEM_H
