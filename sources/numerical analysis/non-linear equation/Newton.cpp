#include "Equation.h"

float Newton(std::function<float(float)> function, std::function<float(float)> derivate, float begin, float tol)
{
    float next = begin + 1;

    while(true)
    {
        next = begin - function(begin) / derivate(begin);

        if(fabsf(begin - next) > tol)
        {
            return next;
        }
        else
        {
            begin = next;
        }
    }
}