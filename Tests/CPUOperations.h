#ifndef CPUOPERATIONS_H
#define CPUOPERATIONS_H
#include "Test.h"
#include "types.h"

namespace CPU {
template <typename NumberType>
class CPUOperations : public TestAbstract
{
public:
    CPUOperations() = default;

public:
    virtual void startTest() override;
    virtual void preparationBeforeTest() override;
    virtual void preparationAfterTest() override;

private:
    void testTemplateValue();
};

template <typename NumberType>
void CPUOperations<NumberType>::startTest()
{
    this->testTemplateValue();
}

template <typename NumberType>
void CPUOperations<NumberType>::preparationBeforeTest()
{

}

template <typename NumberType>
void CPUOperations<NumberType>::preparationAfterTest()
{
}

template<typename NumberType>
void CPUOperations<NumberType>::testTemplateValue()
{
    static constexpr auto limit = 1024*256*32;

#define ValName $$$
#define def ValName += ValName; ValName -= ValName; \
    ValName *= ValName; ValName /= 3; \
    ++ValName; --ValName;

    NumberType ValName = static_cast<NumberType>(123);

    for (register int i = 0; i < limit; ++i)
    {
        def       def def def
    def def       def     def
        def       def     def
    def def def   def def def
    }
}

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
