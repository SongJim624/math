#include "../math.h"
using namespace math;

void add(size_t length, const double * left, const double * right, double * results)
{
    for(size_t i = 0; i < length; ++i)
    {
        *(results++) = *(left++) + *(right++);
    }
}