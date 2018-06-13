#ifndef RAMREADWRITESPEED_H
#define RAMREADWRITESPEED_H
#include "Test.h"
#include "types.h"

class RAMReadWriteSpeed : public TestAbstract
{
    int_fast64_t *_buffer;
public:
    virtual void startTest() override;
    virtual void preparationBeforeTest() override;
    virtual void preparationAfterTest() override;

    static int64_t getTestDataSizeInMBytes();
};

#endif // RAMREADWRITESPEED_H
