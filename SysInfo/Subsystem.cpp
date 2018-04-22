#include "Subsystem.h"
#include "StringUtilities.h"
#include <fstream>
#include <cstring>
#include <chrono>
#include <stdlib.h>
using namespace std;
using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

Subsystem::Subsystem()
{

}

struct sysinfo Subsystem::getGlobalInfo()
{
    struct sysinfo sys;
    memset(&sys, 0, sizeof(sys));

    int status = sysinfo(&sys);

    return sys;
}

string Subsystem::getPrintableInfo(const ListInfoT &src, const StringSet &filter) const
{
    auto callbackIsPrintable = [&filter] (const string &condition) -> bool {
        return (filter.find(condition) != filter.end() || filter.empty());
    };

    // finder maxlen str
    int maxlen = 0;
    for (const auto &pairline : src)
        if (pairline.first.size() > maxlen)
            maxlen = pairline.first.size();

    string spaces;
    for (int i = 0; i < maxlen; i++)
        spaces += ' ';

    string result;
    for (const auto &pairline : src) {
        if (callbackIsPrintable(pairline.first)) {
            if (pairline.first == "processor")
                result += '\n';
            result += pairline.first;
            result.append(spaces.begin(), spaces.begin() + (maxlen-pairline.first.size()+1));
            result += " : ";
            result += (pairline.second + '\n');
        }
    }

    return result;
}

string::size_type getPosNoValidSymbol(const string &src, int start, int step, char symForEqual) {
    for (int i = start+step; i >= 0 && i < src.size(); i += step)
        if (src[i] != symForEqual)
            return i - (step < 0 ? step : 0);

    return string::npos;
}

ListInfoT Subsystem::getStructureInfo(const string &src) const
{
    ListInfoT listinfo;

    StringList lines = StringUtil::cropToStrings(src, '\n');

    for (auto &line : lines) {
        line = StringUtil::deleteSymbols(line, "\t\n");
    }

    for (const auto &line : lines) {

        if (line.empty())
            continue;

        auto prev = getPosNoValidSymbol(line, line.find(':'), -1, ' ');
        auto next = getPosNoValidSymbol(line, line.find(':'),  1, ' ');

        if (prev == string::npos || next == string::npos)
            continue;

        listinfo.push_back({line.substr(0, prev), line.substr(next, line.length()-1)});

    }

    return std::move(listinfo);
}

using pos_type = ifstream::pos_type;

pos_type getFileSize(ifstream &src) {
    pos_type storePos = src.tellg();
    src.seekg(0, ios::end);
    pos_type endFilePos = src.tellg();
    src.seekg(storePos);
    return endFilePos;
}

std::string Subsystem::_getDataFromFile(const std::string &pathToFile)
{
    const string RFILE = "rfile";
    const string comm = string("cat ") + pathToFile + " >> " + RFILE;
    system(comm.c_str());

    string result;

    {
        ifstream file(RFILE, std::ios::in);

        if (!file.is_open()) {
            cout << "failed read file: " << pathToFile;
            return std::move(result);
        }
        auto sz = getFileSize(file);

        result.resize(sz);

        file.read((char*)result.data(), sz);
        file.close();
    }

    string REM_RFILE = string("rm ") + RFILE;
    system(REM_RFILE.c_str());

    return std::move(result);

}

string Subsystem::_getDataFromCommand(const string &command)
{
    string filename = "tmpfile";
    system(((command + " > ") + filename).c_str());
    string result = _getDataFromFile(filename);
    system((string("rm ") + filename).c_str());

    return result;
}

string Subsystem::_alignText(const string &str)
{
    return str;
//    auto pos = str.find_first_not_of(' ');

//    string result;

//    for (const auto &line : StringUtil::cropToStrings(str, '\n'))
//        (result += line.substr(pos)) += '\n';

    //    return result;
}

double Subsystem::_getItervalBetweenCalculate(const std::function<void ()> &func)
{
    auto start = steady_clock::now();
    // calculating...
    func();
    // end...
    auto end = steady_clock::now();
    return ((double)duration_cast<milliseconds>(end - start).count()) / 1000;
}
