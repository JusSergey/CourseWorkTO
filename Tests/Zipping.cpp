#include "Zipping.h"
#include "../utils.h"

///////////////////////////////
// IMPLEMENTATION COMPRESS PART
void ZipCompress::startTest() {
    compress();
}

void ZipCompress::preparationBeforeTest()
{
    if (!Utils::FileExist(DEFAULT_IN_FILE))
        generateFile();
}

void ZipCompress::preparationAfterTest()
{

}

void ZipCompress::generateFile(size_t MBytes)
{
    std::string command = Utils::splitArgs("dd if=/dev/urandom of=", DEFAULT_IN_FILE, " count=1048576", " bs=", MBytes);
    system(command.c_str());
}

void ZipCompress::compress(const char *filename)
{
    std::string command = Utils::splitArgs("zip ", DEFAULT_ZIP_FILE, " ", filename, " -9");
    system(command.c_str());
}



/////////////////////////////////
// IMPLEMENTATION UNCOMPRESS PART
void ZipUncompress::startTest() {
    uncompress();
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
