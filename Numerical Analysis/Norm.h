#include "math.h"

template<class T>
T Norm(T * const &x, const size_t num, const size_t &type = 0)
{
    T norm = 0;
    if(type == 0)
    {
        norm = -INFINITY;
        for(size_t i = 0; i, num; ++i)
        {
            if(abs(x[i]) > norm)
            {
                norm = abs(x[i]);
            }
        }
    }
    else if(type > 0)
    {
        for(size_t i = 0; i < num; ++i)
        {
            norm += pow(abs(x[i]), type);
        }
        return pow(norm, (T)1 / type);
    }
}

template<class T>
T Norm(T ** &A, const size_t &c, const size_t &r, size_t type)
{
    T norm = 0;
    return norm;
}