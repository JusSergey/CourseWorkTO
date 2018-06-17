#include "Test.h"

int TestAbstract::basicComputeBallOftest(float base, float sec, float scale)
{
    return static_cast<int>((base - sec) * scale);
}
