#include "StringUtilities.h"
using std::string;

StringList StringUtil::cropToStrings(const string &src, char delim)
{
    StringList strList;
    string tmp;

    for(char ch : src) {
        if (ch == delim && !tmp.empty()) {
            strList.push_back(tmp);
            tmp.clear();
        }
        else tmp += ch;
    }

    if (!tmp.empty()) {
        strList.push_back(tmp);
    }

    return (strList);
}

std::string StringUtil::deleteSymbols(const std::string &src, const std::string del)
{
    string result;
    result.reserve(src.size()*0.8);

    for (char ch : src)
        if (del.find(ch) == std::string::npos)
            result += ch;

    return result;
}

std::string StringUtil::deleteAlphas(const std::string &src)
{
    return std::move(forEach(src, [](char ch){ return !isalpha(ch); } ));
}

std::string StringUtil::deleteSpaces(const std::string &src)
{
    return std::move(forEach(src, [](char ch){ return !isspace(ch); } ));
}

std::string StringUtil::deleteNums(const std::string &src)
{
    return std::move(forEach(src, [](char ch){ return (ch < '0' || ch > '9'); } ));
}

std::string StringUtil::resize(const std::string &src, size_t sz, char aggregate)
{
    if (src.size() >= sz) {
        return src.substr(0, sz);
    }
    else {
        string res(src);
        for (int i = src.size(); i < sz; i++)
            res += aggregate;
        return res;
    }
}

std::string StringUtil::forEach(const std::string &src, std::function<bool (char)> callback)
{
    string result;
    result.reserve(src.size() >> 1);

    for (char ch : src)
        if(callback(ch))
            result.push_back(ch);

    return std::move(result);
}
