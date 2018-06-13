#ifndef HARDDRIVESPEED_H
#define HARDDRIVESPEED_H
#include "Test.h"
#include "types.h"

class HardDriveSpeed
{
protected:
    static vector<char> _buffer;
public:
    static void generateRandomFile(const size_t MBytes, string filename);

    // fast
    static void generateFastFile(const size_t MBytes, string filename);
};

//
class HardDriveSpeedWrite : public TestAbstract, public HardDriveSpeed {
    int64_t _szFile;
public:
    HardDriveSpeedWrite(int64_t szFile) : _szFile(szFile) {}
    virtual void startTest() override;
    virtual void preparationBeforeTest() override;
    virtual void preparationAfterTest() override;
};

//
class HardDriveSpeedRead : public TestAbstract, public HardDriveSpeed {
public:
    virtual void startTest() override;
    virtual void preparationBeforeTest() override;
    virtual void preparationAfterTest() override;
};

#endif // HARDDRIVESPEED_H
