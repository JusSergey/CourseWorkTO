#include "HardDriveSpeed.h"
#include "utils.h"
#include <cstdlib>
#include <stdio.h>
#include <fstream>
using std::ofstream;
using std::ifstream;

vector<char> HardDriveSpeed::_buffer;

static constexpr const char* DEFAULT_TEST_WRITE_FILENAME = "filename.bin";


void HardDriveSpeed::generateRandomFile(const size_t MBytes, std::string filename)
{
    std::string command = Utils::splitArgs("dd if=/dev/urandom of=", filename, " count=1048576", " bs=", MBytes);
    system(command.c_str());
}

void HardDriveSpeed::generateFastFile(const size_t MBytes, std::string filename)
{
    _buffer.resize(1048576);
    ofstream ofWrite(filename);

    if (!ofWrite.is_open()) {
        std::cerr << "cannot open file: " << filename <<
                     " for write [void HardDriveSpeed::generateZeroFile(size_t MBytes, std::string filename)]\n";
        return;
    }

    int64_t writeBytes(0);
    const int64_t allBytes = MBytes*1048576;

    while (writeBytes < allBytes) {
        const int64_t bs = (allBytes - writeBytes > _buffer.size() ? _buffer.size() : allBytes - writeBytes);
        ofWrite.write(_buffer.data(), bs);
        writeBytes += bs;
    }

    ofWrite.flush();
    ofWrite.close();
}

//////////////////////////////////////////////
void HardDriveSpeedWrite::startTest()
{
    HardDriveSpeed::generateFastFile(_szFile, DEFAULT_TEST_WRITE_FILENAME);
}

void HardDriveSpeedWrite::preparationBeforeTest()
{

}

void HardDriveSpeedWrite::preparationAfterTest()
{

}

void HardDriveSpeedWrite::setBallForThisTest(ResultTest &test)
{
    const float MBytes = (float)_szFile;
    test.ball = static_cast<int>(MBytes / test.sec / 10);
    cout << "HDWRITE: " << test.ball << std::endl;
}


//////////////////////////////////////////////
void HardDriveSpeedRead::startTest()
{
    struct stat st;
    stat(DEFAULT_TEST_WRITE_FILENAME, &st);

    _buffer.resize(1048576 * 32);
    ifstream ifRead(DEFAULT_TEST_WRITE_FILENAME);
    std::cout << "filename: " << DEFAULT_TEST_WRITE_FILENAME << std::endl;

    if (!ifRead.is_open()) {
        std::cerr << "cannot open file: " << DEFAULT_TEST_WRITE_FILENAME <<
                     " for read [void HardDriveSpeed::generateZeroFile(size_t MBytes, std::string filename)]\n";
        return;
    }

    const int64_t allBytes = st.st_size;
    std::cout << "sz read: " << allBytes / 1048576 << std::endl;

    while (wasBeenRead < allBytes) {
        const int64_t bs = (allBytes - wasBeenRead > _buffer.size() ? _buffer.size() : allBytes - wasBeenRead);
        ifRead.read(_buffer.data(), bs);
        wasBeenRead += bs;
    }

    ifRead.close();
}

void HardDriveSpeedRead::preparationBeforeTest()
{
    wasBeenRead = 0;
    system("free && sync && echo 3 > /proc/sys/vm/drop_caches && free");
}

void HardDriveSpeedRead::preparationAfterTest()
{
    remove(DEFAULT_TEST_WRITE_FILENAME);
}

void HardDriveSpeedRead::setBallForThisTest(ResultTest &test)
{
    //wasBeenRead - in bytes. wasBeenRead/1048576 is meeans as convert to MBytes
    const float MBytes = (float)wasBeenRead / 1048576;
    test.ball = static_cast<int>(MBytes / test.sec / (2048/100));
    cout << "HDREAD: " << test.ball << std::endl;
}
