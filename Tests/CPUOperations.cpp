#include "CPUOperations.h"
#include <random>
#include <ctime>

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
