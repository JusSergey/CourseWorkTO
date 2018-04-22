#include "SubsystemRAM.h"
#include "StringUtilities.h"

string SubsystemRAM::info;

SubsystemRAM* SubsystemRAM::instance = nullptr;

std::string SubsystemRAM::getPrintableInfo() const
{
    if (info.empty()) {
        system("./lshw -c memory > memoryinfo");
        info = _getDataFromFile("memoryinfo");
        system("rm memoryinfo");

        info = info.substr(info.find("*-memory"));
    }
    return info;
}

ActiveDev::TestResult SubsystemRAM::test()
{
    static constexpr size_t SIZE = 1024*1024 * 128;
    char *buff = new char[SIZE];

    auto delay_time_rw = _getItervalBetweenCalculate([&]{
        for (int n = 0; n < 16; ++n){
            for (register size_t i = 0; i < SIZE; ++i)
                buff[i] = 'c';

            for (register size_t i = SIZE-1; i > -1; --i)
                buff[i] = 'a';
        }
    });

    delete []buff;
    return std::make_pair(string("speed read/write: ") + Number<float>::toStr(((double)SIZE*32)/1024/1024/1024/delay_time_rw) + " GB/sec", delay_time_rw);
}

