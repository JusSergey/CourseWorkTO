#include "SubsystemOS.h"

// default values of OS-children
OS::Processes *OS::Processes::instance = nullptr;
OS::Modules *OS::Modules::instance = nullptr;
OS::RAMMemory *OS::RAMMemory::instance = nullptr;


string OS::Processes::getPrintableInfo() const
{
    system("./ps -A > processes");
    auto info = _getDataFromFile("processes");
    system("rm processes");

    return info;
}

string OS::Modules::getPrintableInfo() const
{
    system("./lsmod > modules");
    auto info = _getDataFromFile("modules");
    system("rm modules");

    return info;
}

string OS::RAMMemory::getPrintableInfo() const
{
    return Subsystem::getPrintableInfo(getDeviceInfo(), {"MemTotal", "MemFree", "MemAvailable",
                                                         "Buffers", "Cached", "SwapCached", "SwapTotal",
                                                         "SwapFree", "Active", "PageTables"});
}

ListInfoT OS::RAMMemory::getDeviceInfo() const
{
    ListInfoT listinfo;

    string all = _getDataFromFile(PATH_MEMORY_INFO);

    StringList lines = StringUtil::cropToStrings(all, '\n');

    for (auto &line : lines) {
        line = StringUtil::deleteSymbols(line, " \t\n");
    }

    for (const auto &line : lines) {
        StringList simpleList = StringUtil::cropToStrings(line, ':');
        if (simpleList.size() == 2) {
            auto i = simpleList.begin();
            auto first = *i;
            auto second = *++i;
            listinfo.push_back(PairType(first, second));
        }
    }

    return std::move(listinfo);
}
