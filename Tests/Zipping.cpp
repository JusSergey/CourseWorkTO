#include "Zipping.h"
#include "../utils.h"

// for remove()
#include <cstdio>

static constexpr const char *DEFAULT_IN_FILE = "raw_data.bin";
static constexpr const char *DEFAULT_ZIP_FILE= "compressed.zip";
static constexpr const size_t DEFAULT_IN_FILE_SIZE = 256;


///////////////////////////////
// IMPLEMENTATION COMPRESS PART
void ZipCompress::startTest() {
    compress(DEFAULT_IN_FILE);
}

void ZipCompress::preparationBeforeTest()
{
    if (!Utils::FileExist(DEFAULT_IN_FILE)){
        cout << "FILE WILL BE CREATED";
        generateFile(DEFAULT_IN_FILE_SIZE);
    }
}

void ZipCompress::preparationAfterTest()
{

}

void ZipCompress::setBallForThisTest(ResultTest &test)
{
    test.ball = float(DEFAULT_IN_FILE_SIZE) / (float(DEFAULT_IN_FILE_SIZE) / 100.F) / test.sec * 6.F;
}

void ZipCompress::generateFile(size_t MBytes)
{
    HardDriveSpeed::generateRandomFile(MBytes, DEFAULT_IN_FILE);
}

void ZipCompress::compress(const char *filename)
{
    std::string command = Utils::splitArgs("zip ", DEFAULT_ZIP_FILE, " ", filename);
    cout << "compress command: " << command << std::endl;
    system(command.c_str());
}



/////////////////////////////////
// IMPLEMENTATION UNCOMPRESS PART
void ZipUncompress::startTest() {
    uncompress(DEFAULT_ZIP_FILE);
}

void ZipUncompress::preparationBeforeTest()
{

}

void ZipUncompress::preparationAfterTest()
{
    remove(DEFAULT_ZIP_FILE);
    remove(DEFAULT_IN_FILE);
}

void ZipUncompress::setBallForThisTest(ResultTest &test)
{
    test.ball = float(DEFAULT_IN_FILE_SIZE) / (float(DEFAULT_IN_FILE_SIZE) / 100.F) / test.sec * 1.1F;
}

void ZipUncompress::uncompress(const char *filename)
{
    std::string command = Utils::splitArgs("unzip ", filename, " -d ./");
    remove(DEFAULT_IN_FILE);
    system(command.c_str());
}

//////////////////////////////////
/// \brief ZipAbstract::getSizeFile
/// \return filesize for compress/uncompress
///
size_t ZipAbstract::getSizeFile()
{
    return DEFAULT_IN_FILE_SIZE;
}
