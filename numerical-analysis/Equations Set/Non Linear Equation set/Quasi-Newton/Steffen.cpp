#include "stddef.h"
#include "math.h"
#include <vector>
#include <functional>

std::vector<float> Steffen(std::function<std::vector<float>(std::vector<float>&)>& function, std::vector<float> begin)

typedef (double *pf)(double const * , const size_t &num)
void Steffen(pf const *Fun, double * const x, const size_t &num)
{
    double * f = new double [num];
    double ** A = new double [num];
    for(size_t i = 0; i < num; ++i)
    {
        A[i] = new double[num];
    }

    for(size_t i = 0; i < num; ++i)
    {
       f[i] = Fun[i](x, num);
    }



    
}