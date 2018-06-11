#include "SubsystemOS.h"
#include "Utils/HtmlUtils.h"

// default values of OS-children
OS::Processes *OS::Processes::instance = nullptr;
OS::Modules *OS::Modules::instance = nullptr;
OS::RAMMemory *OS::RAMMemory::instance = nullptr;


string OS::Processes::getPrintableInfo() const
{
    system("./ps -x > processes");
    auto info = _getDataFromFile("processes");
    system("rm processes");

    return info;
}

ListStrVects OS::Processes::getDeviceInfo() const
{
    auto strLines = StringUtil::cropToStrings(getPrintableInfo(), '\n');

    ListStrVects result;

    for (auto &allData : strLines) {

        if (allData.find(':') == string::npos) {
            auto strList = StringUtil::cropToStrings(allData, ' ');
            StringVector vec;
            vec.insert(vec.end(), strList.begin(), strList.end());
            result.push_back(std::move(vec));
            continue;
        }

        // size must be 2
        auto sublines = StringUtil::cropToStrings(allData, ':');
        StringVector oneLine;

        //begin
        auto firstPart = sublines.front().substr(0, sublines.front().find_last_of(' '));
        auto cont = StringUtil::cropToStrings(firstPart, ' ');
        oneLine.insert(oneLine.end(), cont.begin(), cont.end());

        // medium
        auto pospp = allData.find(':');
        auto medium = StringUtil::getWord(allData, pospp, " \t");
        oneLine.push_back(medium);

        //end
        auto firNot = allData.find_first_not_of(' ', pospp);
        auto lastPart = StringUtil::cropToStrings(allData.substr(firNot), ' ');
        oneLine.insert(oneLine.end(), lastPart.begin(), lastPart.end());

        result.push_back(oneLine);
    }

    return result;
}

string OS::Modules::getPrintableInfo() const
{
    system("./lsmod > modules");
    auto info = _getDataFromFile("modules");
    system("rm modules");

    return info;
}

ListStrVects OS::Modules::getDeviceInfo() const
{
    ListStrVects result;
    for (const auto &line : StringUtil::cropToStrings(getPrintableInfo(), '\n')) {
         result.push_back(StringUtil::strListToStrVec(StringUtil::cropToStrings(line, ' ')));
    }

    return result;

}

string OS::RAMMemory::getPrintableInfo() const
{
    return Subsystem::getPrintableInfo(getDeviceInfo(), {"MemTotal", "MemFree", "MemAvailable",
                                                         "Buffers", "Cached", "SwapCached", "SwapTotal",
                                                         "SwapFree", "Active", "PageTables"});
}

ListStrVects OS::RAMMemory::getDeviceInfo() const
{
    ListStrVects listinfo;

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
            listinfo.push_back(StringVector({first, second}));
        }
    }

    return std::move(listinfo);
}

string OS::SubOS::getHTMLCode() const {
    auto data = getDeviceInfo();
    HtmlUtils::InfoGenerateFile inpuInfo;
    inpuInfo.border_width = 1;
    inpuInfo.collapse_border = true;
    inpuInfo.headTitle = data.front();
    for (auto str : inpuInfo.headTitle) {
        cout << '[' << str << (str[0] == ' ') << ']';
    }
    cout << std::endl;
    data.pop_front();
    inpuInfo.data = std::move(data);
    inpuInfo.pageTitle = "DEF_PG_TLT";
    return HtmlUtils::createAndGetHtmlPage(inpuInfo);
}

