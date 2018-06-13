#ifndef ZIPPING_H
#define ZIPPING_H
#include "Test.h"
#include "HardDriveSpeed.h"


// THE TEST FOR DEFINE SPEED OF COMPRESS
class ZipCompress : public TestAbstract
{

    friend class ZipUncompress;
public:
    ZipCompress() = default;
    virtual void startTest() override;
    virtual void preparationBeforeTest() override;
    virtual void preparationAfterTest() override;

private:
    void generateFile(size_t MBytes);
    void compress(const char *filename);
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
    void uncompress(const char *filename);
};

#endif // ZIPPING_H
