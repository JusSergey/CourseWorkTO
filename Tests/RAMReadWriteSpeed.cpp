#include "RAMReadWriteSpeed.h"
static constexpr int_fast64_t SIZE_ARR = (1024*1024 * 16);
static constexpr int_fast64_t ITERATIONS_TEST = (16 * 8);

void RAMReadWriteSpeed::startTest()
{
    int_fast64_t byte = 'b';
    for (int_fast64_t n = 0; n < ITERATIONS_TEST; ++n) {
        for (register int_fast64_t i = 0; i < SIZE_ARR-2; ++i) {
            _buffer[i] = byte;
        }
    }
}

void RAMReadWriteSpeed::preparationBeforeTest()
{
    _buffer = new int_fast64_t[SIZE_ARR + 1];
}

void RAMReadWriteSpeed::preparationAfterTest()
{
    delete []_buffer;
}

int64_t RAMReadWriteSpeed::getTestDataSizeInMBytes()
{
    return (SIZE_ARR * ITERATIONS_TEST) / 1048576 * sizeof(int_fast64_t);
}
