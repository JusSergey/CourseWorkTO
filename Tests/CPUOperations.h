#ifndef CPUOPERATIONS_H
#define CPUOPERATIONS_H
#include "Test.h"
#include "types.h"

namespace CPU {
template <typename NumberType>
class CPUOperations : public TestAbstract
{
public:
    CPUOperations();

public:
    virtual void startTest() override;
    virtual void preparationBeforeTest() override;
    virtual void preparationAfterTest() override;

private:
    void testTemplateValue();
};

class AbstractCache : public TestAbstract {
protected:
    vector<int_fast32_t> indexToCellRAM;
    char **buffer;
protected:
    void cacheTest();
    virtual void fillIndexContainer() = 0;
public:
    virtual void preparationBeforeTest() override;
    virtual void preparationAfterTest() override;
    virtual void startTest() override;
};

class Cache : public AbstractCache {
public:
    Cache() = default;
    ~Cache() = default;
    virtual void fillIndexContainer();
};

class NoUseCache : public AbstractCache {
public:
    NoUseCache() = default;
    ~NoUseCache() = default;
    virtual void fillIndexContainer();
};

}

#endif // CPUOPERATIONS_H
