#include "PDF.h"

void PDF::Exponential(const float &lamada, const float &x, float &pdf)
{
    if(x > 0)
    {
        pdf = lamada * expf(-lamada * x);
    }
    else
    {
        pdf = 0.0;
    }
    
}