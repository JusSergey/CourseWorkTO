#ifndef STRINGUTILITIES_H
#define STRINGUTILITIES_H

#include <functional>
#include <string>
#include <list>
#include <sstream>
#include "types.h"

template <typename T = int>
class Number {
public:
    static T toNum(const std::string &num) {
        std::stringstream oss;
        oss << num;
        T resultNumber;
        oss >> resultNumber;
        return resultNumber;
    }
    static std::string toStr(T num) {
        std::stringstream oss;
        oss << num;
        std::string resultNumber;
        oss >> resultNumber;
        return resultNumber;
    }
};

class StringUtil
{
public:
    static StringVector strListToStrVec(const StringList &strList);
    static StringList cropToStrings(const std::string &src, char delim);
    static std::string deleteSymbols(const std::string &src, const std::string del);
    static std::string deleteAlphas(const std::string &src);
    static std::string deleteSpaces(const std::string &src);
    static std::string deleteNums(const std::string &src);
    static std::string getWord(const std::string &src, size_t pos, const std::string &limitsChars);

    static std::string resize(const std::string &src, size_t sz, char aggregate = ' ');
    static std::string removeRepeatChars(const std::string &src, char remSymbol);

private:
    static std::string forEach(const std::string &src, std::function<bool(char)>);
};

#endif // STRINGUTILITIES_H
