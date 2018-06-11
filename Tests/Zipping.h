#ifndef ZIPPING_H
#define ZIPPING_H
#include "Test.h"

static constexpr const char *DEFAULT_IN_FILE = "raw_data.bin";
static constexpr const char *DEFAULT_ZIP_FILE= "compressed.zip";
static constexpr const size_t DEFAULT_IN_FILE_SIZE = 1024;

// THE TEST FOR DEFINE SPEED OF COMPRESS
class ZipCompress : public TestAbstract
{
public:
    ZipCompress() = default;
    virtual void startTest() override;
    virtual void preparationBeforeTest() override;
    virtual void preparationAfterTest() override;

private:
    void generateFile(size_t MBytes = DEFAULT_IN_FILE_SIZE);
    void compress(const char *filename = DEFAULT_IN_FILE);
};


// THE TEST FOR DEFINE SPEED OF UNCOMPRESS
class ZipUncompress : public TestAbstract
{
public:
    ZipUncompress() = default;
    virtual void startTest() override;
    virtual void preparationBeforeTest() override;
    virtual void preparationAfterTest() override;

private:
    void uncompress(const char *filename = DEFAULT_ZIP_FILE);
};

#endif // ZIPPING_H
