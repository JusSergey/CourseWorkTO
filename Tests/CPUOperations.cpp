#include "CPUOperations.h"
#include <random>
#include <ctime>

#define SIZE (1024*8)
#define SIZE_SIZE (SIZE * SIZE)

void CPU::AbstractCache::cacheTest()
{
    register int i, j, k = 0;
    for (i = 0; i < SIZE; ++i){
        for (j = 0; j < SIZE; ++j){
            buffer[ indexToCellRAM[k] ][ indexToCellRAM[k+1] ] = 0xABCDEF;
            ++k;
        }
    }
}

void CPU::AbstractCache::preparationBeforeTest()
{
    buffer = new int_fast64_t*[SIZE];
    for (int i = 0; i < SIZE; ++i)
        buffer[i] = new int_fast64_t[SIZE];

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

size_t CPU::AbstractCache::getSizeComputingDataMBytes()
{
    return SIZE*SIZE*sizeof(**buffer) / 1048576 + SIZE*SIZE*sizeof(indexToCellRAM[0])*2 / 1048576 + SIZE*SIZE*sizeof(int) / 1048576;
}

void CPU::NoUseCache::fillIndexContainer()
{
    indexToCellRAM.resize(SIZE_SIZE+1);
    std::mt19937 gen;
    gen.seed(time(0));
    for (size_t i = 0; i < indexToCellRAM.size(); ++i) {
        indexToCellRAM[i] = gen() % SIZE;
    }
}

void CPU::NoUseCache::setBallForThisTest(ResultTest &test)
{
    test.ball = 90.F / test.sec;
}

void CPU::UseCache::fillIndexContainer()
{
    indexToCellRAM.resize(SIZE_SIZE+1);
    for (size_t i = 0; i < indexToCellRAM.size(); ++i) {
        indexToCellRAM[i] = i % SIZE;
    }
}

void CPU::UseCache::setBallForThisTest(ResultTest &test)
{
    test.ball = 50.F / test.sec;
}
