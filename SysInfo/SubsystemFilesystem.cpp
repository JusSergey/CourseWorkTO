#include "SubsystemFilesystem.h"
#include <sys/ioctl.h>
#include <linux/hdreg.h>
#include <fstream>
using namespace std;

SubsystemFilesystem *SubsystemFilesystem::instance = nullptr;

std::string SubsystemFilesystem::readDeviceInfo()
{

}

ActiveDev::TestResult SubsystemFilesystem::test()
{

}

std::string SubsystemFilesystem::getPrintableInfo() const
{
    const string path = "info_hard_drive";

    system((string("fdisk -l /dev/sda >> ") + path).c_str());

    auto result = _getDataFromFile(path);

    system((string("rm ") + path).c_str());

    return result;
}

static constexpr size_t SIZE_FILE = 1024*1024 * 512;
static constexpr size_t BLCK_FILE = 8;

double SubsystemFilesystem::testWrite()
{
    char *buffer = new char[SIZE_FILE / BLCK_FILE];

    auto delay_time = _getItervalBetweenCalculate([&]{
        ofstream wrf("test_file");
        if (!wrf.is_open())
            throw ("can't create file for test speed write (double SubsystemFilesystem::testWrite())");

        for (size_t i = 0; i < BLCK_FILE; ++i)
            wrf.write(buffer, SIZE_FILE/BLCK_FILE);

        wrf.flush();
        wrf.close();

    });

    (cout << "delay_write: " << delay_time << '\n').flush();

    delete []buffer;
    return delay_time;
}

double SubsystemFilesystem::testRead()
{


    char *buffer = new char[SIZE_FILE / BLCK_FILE];
    bool isEnableFileCache = false;

    auto delay_time = _getItervalBetweenCalculate([&]{

        isEnableFileCache = _getDataFromCommand("/sbin/sysctl vm.drop_caches=3").empty();

        ifstream rdf("test_file");

        if (!rdf.is_open()){
            throw ("can't create file for test speed read (double SubsystemFilesystem::testRead())");
        }

        for (size_t i = 0; i < BLCK_FILE; ++i)
            rdf.read(buffer, SIZE_FILE/BLCK_FILE);

        rdf.close();
    });

    if (isEnableFileCache)
        (cout << "CACHE_ENABLE was TRUE..  test read was failed\n ").flush();
    else
        (cout << "CACHE_ENABLE was FALSE.. test read was success\n").flush();

    delete []buffer;
    return delay_time;
}
