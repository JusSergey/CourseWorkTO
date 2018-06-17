#include "NetUtils.h"
#include "StringUtilities.h"
#include "SysInfo/Subsystem.h"
#include <unistd.h>

std::string NetUtils::getHostName()
{
    char hostnameBuffer[128] {0};
    gethostname(hostnameBuffer, sizeof(hostnameBuffer)-1);
    return hostnameBuffer;
}

std::string NetUtils::getLocalIP()
{
    string dataFromFile = Subsystem::_getDataFromCommand("ifconfig | grep 'inet addr'");
    string ip;
    for (auto str : StringUtil::cropToStrings(dataFromFile, '\n')) {
        auto lines = StringUtil::cropToStrings(str, ' ');
        if (lines.size() == 4) {
            const string InetAddr = "inet addr:";
            auto pos = str.find(InetAddr);
            if (pos != string::npos) {
                ip = StringUtil::cropToStrings(str.substr(pos + InetAddr.size()), ' ').front();
            }
        }
    }

    return ip;
}

bool NetUtils::isValidPort(const std::string &src)
{
    if (!StringUtil::deleteNums(src).empty() ||
            Number<int>::toNum(src) > 65535  ||
            Number<int>::toNum(src) == 0)
    {
        return false;
    }

    return true;
}

bool NetUtils::isValidIPv4(const std::string &src)
{
    auto lines(StringUtil::cropToStrings(src, '.'));
    if (lines.size() != 4)
        return false;

    for (string num : lines) {
        if (Number<int>::toNum(num) > 255)
            return false;
    }

    return StringUtil::deleteSymbols(StringUtil::deleteNums(src), ".").empty();
}
