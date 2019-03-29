#include "math.h"

inline float H0(const float &x)
{
    return (x * x - 2 * fabsf(x) + 1) * (2 * fabsf(x) + 1);
}

inline float H1(const float &x)
{
    return fabsf(x) * (x * x  - 2 * fabsf(x) + 1);
}

inline float L0(const float &x)
{
    return 1 - fabsf(x); 
}