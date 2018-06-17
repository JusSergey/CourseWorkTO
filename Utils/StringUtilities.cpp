#include "StringUtilities.h"
using std::string;

StringVector StringUtil::strListToStrVec(const StringList &strList)
{
    StringVector strVec;
    strVec.insert(strVec.end(), strList.begin(), strList.end());
    return strVec;
}

StringList StringUtil::cropToStrings(const string &src, char delim)
{
    StringList strList;
    string tmp;

    for(char ch : src) {
        if (ch == delim) {
            if (!tmp.empty()) {
                strList.push_back(tmp);
                tmp.clear();
            }
        }
        else tmp += ch;
    }

    if (!tmp.empty()) {
        strList.push_back(tmp);
    }

    return strList;
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

std::string StringUtil::getWord(const std::string &src, size_t pos, const std::string &limitsChars)
{
    auto isContained = [&](char ch, const std::string &findList) -> bool {
        for (char symbol : findList)
            if (symbol == ch)
                return true;

        return false;
    };

    size_t minpos = -1, maxpos = src.size();

    for (auto &rval : { std::pair<size_t &, int>(minpos, -1),
                        std::pair<size_t &, int>(maxpos,  1)})
    {
        size_t limit = rval.second < 0 ? -1 : src.size();
        for (size_t currPos = pos; currPos != limit; currPos--) {
            if (!isContained(src[pos], limitsChars)) {
                rval.first = currPos;
                break;
            }
        }
    }

    return src.substr(minpos+1, maxpos-1);
}

std::string StringUtil::resize(const std::string &src, size_t sz, char aggregate)
{
    if (src.size() >= sz) {
        return src.substr(0, sz);
    }
    else {
        string res(src);
        for (int i = src.size(); i < sz; i++) {
            res += aggregate;
        }
        return res;
    }
}

std::string StringUtil::removeRepeatChars(const std::string &src, char remSymbol)
{
    bool wasFinded(false);
    std::string result = forEach(src, [&](char symbol) -> bool {
        if (symbol == remSymbol) {
            if (wasFinded) {
                return false;
            }
            wasFinded = true;

        } else {
            wasFinded = false;
        }
        return true;
    });

    return result;
}

bool StringUtil::isValidIPv4(const std::string &src)
{
    auto lines(cropToStrings(src, '.'));
    if (lines.size() != 4)
        return false;

    for (string num : lines) {
        if (Number<int>::toNum(num) > 255)
            return false;
    }

    return StringUtil::deleteSymbols(StringUtil::deleteNums(src), ".").empty();
}

bool StringUtil::isValidPort(const std::string &src)
{
    if (!StringUtil::deleteNums(src).empty() ||
         Number<int>::toNum(src) > 65535     ||
         Number<int>::toNum(src) == 0)
    {
        return false;
    }

    return true;
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
