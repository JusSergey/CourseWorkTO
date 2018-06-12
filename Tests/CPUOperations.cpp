#include "CPUOperations.h"
#include <random>
#include <ctime>

template <typename NumberType>
CPU::CPUOperations<NumberType>::CPUOperations()
{

}

template <typename NumberType>
void CPU::CPUOperations<NumberType>::startTest()
{
    this->testTemplateValue();
}

template <typename NumberType>
void CPU::CPUOperations<NumberType>::preparationBeforeTest()
{

}

template <typename NumberType>
void CPU::CPUOperations<NumberType>::preparationAfterTest()
{

}

template<typename NumberType>
void CPU::CPUOperations<NumberType>::testTemplateValue()
{
    static constexpr auto limit = 1024*1024*32;

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

#define SIZE (1024*16)
#define SIZE_SIZE (SIZE * SIZE)

void CPU::AbstractCache::cacheTest()
{
    register int i, j, k = 0;
    for (i = 0; i < SIZE; ++i){
        for (j = 0; j < SIZE; ++j){
            buffer[ indexToCellRAM[k] ][ indexToCellRAM[k+1] ] = 1;
            ++k;
        }
    }
}

void CPU::AbstractCache::preparationBeforeTest()
{
    buffer = new char*[SIZE];
    for (int i = 0; i < SIZE; ++i)
        buffer[i] = new char[SIZE];

    fillIndexContainer();
}

void CPU::AbstractCache::preparationAfterTest()
{
    for (int i = 0; i < SIZE; ++i)
        delete []buffer[i];

    delete []buffer;
}

void CPU::AbstractCache::startTest()
{
    cacheTest();
}

void CPU::NoUseCache::fillIndexContainer()
{
    indexToCellRAM.resize(SIZE_SIZE+1);
    std::mt19937 gen;
    gen.seed(time(0));
    for (int i = 0; i < indexToCellRAM.size(); ++i) {
        indexToCellRAM[i] = gen() % SIZE;
    }
}

void CPU::Cache::fillIndexContainer()
{
    indexToCellRAM.resize(SIZE_SIZE+1);
    for (int i = 0; i < indexToCellRAM.size(); ++i) {
        indexToCellRAM[i] = i % SIZE;
    }
}
