#ifndef _TEST_H
#define _TEST_H
#include <list>
#include <string>

class TestAbstract
{
public:
    using KeyValue = std::pair<std::string, std::string>;
    using PairList = KeyValue;
    virtual ~TestAbstract() = default;

public:
    virtual void startTest() = 0;
    virtual void preparationBeforeTest() = 0;
    virtual void preparationAfterTest() = 0;

};

#endif // _TEST_H
