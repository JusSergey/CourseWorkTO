#ifndef _TEST_H
#define _TEST_H
#include <list>
#include <string>
#include "types.h"
#include <memory>

struct ResultTest {
    using Ptr = std::unique_ptr<ResultTest>;
    bool finished = false;
    void setInMicrosec(TimeType _mcsec) {
        microsec = _mcsec;
        sec = ((double)(_mcsec / 1000)) / 1000.f;
    }
    //0...100
    int ball = 0.f;
    TimeType microsec = 0;
    float sec = 0;
};

class TestAbstract
{
public:
    virtual ~TestAbstract() = default;

public:
    virtual void startTest() = 0;
    virtual void preparationBeforeTest() = 0;
    virtual void preparationAfterTest() = 0;
    virtual void setBallForThisTest(ResultTest &test) = 0;

protected:
    static int basicComputeBallOftest(float base, float sec, float scale);
};

#endif // _TEST_H
