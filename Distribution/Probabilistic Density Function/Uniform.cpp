#include "PDF.h"

void PDF::Uniform(const float &a, const float &b, const float &x, float &pdf)
{
    if(x > a && x < b)
    {
        pdf = 1.0 / (b - a);
    }
    else
    {
        pdf = 0.0;
    }
    
}