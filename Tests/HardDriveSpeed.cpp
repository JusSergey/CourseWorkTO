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

    int64_t readBytes(0);
    const int64_t allBytes = st.st_size;
    std::cout << "sz read: " << allBytes / 1048576 << std::endl;

    while (readBytes < allBytes) {
        const int64_t bs = (allBytes - readBytes > _buffer.size() ? _buffer.size() : allBytes - readBytes);
        ifRead.read(_buffer.data(), bs);
        readBytes += bs;
    }

    ifRead.close();
}

void HardDriveSpeedRead::preparationBeforeTest()
{
    std::cout << "DROP2";
    system("free && sync && echo 3 > /proc/sys/vm/drop_caches && free");
}

void HardDriveSpeedRead::preparationAfterTest()
{
    remove(DEFAULT_TEST_WRITE_FILENAME);
}
