#ifndef ZIPPING_H
#define ZIPPING_H
#include "Test.h"
#include "HardDriveSpeed.h"

class ZipAbstract : public TestAbstract {
public:
    ZipAbstract() = default;
    static size_t getSizeFile();
};

// THE TEST FOR DEFINE SPEED OF COMPRESS
class ZipCompress : public ZipAbstract
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
class ZipUncompress : public ZipAbstract
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
