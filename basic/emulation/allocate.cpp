#include "../math.h"
using namespace math;

double * math::allocate(size_t length)
{
    return reinterpret_cast<double*>(std::malloc(length * sizeof(double)));
}

void math::free(double * pointer)
{
    std::free(pointer);
}

void math::copy(size_t length, const double * source, size_t step,  double * denstination, size_t increment)
{
    for(size_t i = 0; i < length; ++i)
    {
        *denstination = * source;
        source += step;
        denstination += increment;
    }
}

