#include "SubsystemCPU.h"
#include "../Utils/StringUtilities.h"
#include <iostream>
#include <chrono>
//using std::chrono;

SubsystemCPU* SubsystemCPU::instance = nullptr;
string SubsystemCPU::cache_info;

ListStrVects SubsystemCPU::getDeviceInfo() const
{
    return getStructureInfo(_getDataFromFile(PATH_GENERAL_INFO));
}

//double SubsystemCPU::test()
//{
//    double ball = 0;



//    return ball;
//}

string SubsystemCPU::testIteration() const
{
    // doing 8.000.000.000 iteration (int64_t(8), for don't overflow int)
    static constexpr int64_t limit = int64_t(8)*1000*1000*1000;
    string result = Number<int64_t>::toStr(limit) + "/";

    return result + Number<double>::toStr(_getItervalBetweenCalculate([&]{

        // using register, for calculate only speed CPU, (but not ordered to RAM).
        for (register int64_t i(0); i < limit; ++i)
        {
            // do nothing...
        }
    }));
}

string SubsystemCPU::testCache() const
{
    const int size = 1024*16;

    char **buffer = new char*[size];
    for (int i = 0; i < size; ++i)
        buffer[i] = new char[size];

    char value = (char)255;

    // cache optimisation
    auto optim = _getItervalBetweenCalculate([&]{
        register int i, j;
        for (i = 0; i < size; ++i)
            for (j = 0; j < size; ++j)
                buffer[i][j] = 1;
    });

    // no cache optimisation
    auto nooptim = _getItervalBetweenCalculate([&]{
        register int i, j;
        for (i = 0; i < size; ++i)
            for (j = 0; j < size; ++j)
                buffer[j][i] = 1;
    });

    for (int i = 0; i < size; ++i)
        delete []buffer[i];

    delete []buffer;

    return string("[disable cache: ") + Number<float>::toStr(nooptim) + ", enable cache: " +
            Number<float>::toStr(optim) + "] = " + Number<float>::toStr(nooptim-optim);

}

std::string SubsystemCPU::getPrintableInfo() const
{
    static string s = _getDataFromCommand("./lshw -c processor");
    return s + cache_info;

    return Subsystem::getPrintableInfo(getDeviceInfo(), {"processor", "vendor_id", "cpu family",
                                                         "model name", "microcode"/*, "cpu MHz"*/, "fpu",
                                                         "bogomips", "clflush size", "address sizes"})
            + '\n'
            + cache_info;
}

