#include "Zipping.h"
#include "../utils.h"

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
    if (!Utils::FileExist(DEFAULT_IN_FILE))
        generateFile(DEFAULT_IN_FILE_SIZE);
}

void ZipCompress::preparationAfterTest()
{

}

void ZipCompress::generateFile(size_t MBytes)
{
    HardDriveSpeed::generateRandomFile(MBytes, DEFAULT_ZIP_FILE);
}

void ZipCompress::compress(const char *filename)
{
    std::string command = Utils::splitArgs("zip ", DEFAULT_ZIP_FILE, " ", filename, " -9");
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

}

void ZipUncompress::uncompress(const char *filename)
{
    std::string command = Utils::splitArgs("unzip ", filename, " -d ./");
    system(command.c_str());
}
