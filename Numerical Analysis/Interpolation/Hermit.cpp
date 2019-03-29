//Single value
#include "Interpolation.h"

void Hermite(float * &x, float * &y, float* &dy, const size_t num, float &xi, float &ans)
{
    size_t i = num / 2;
    if(xi > x[i])
    {
        ++i;
        for(; i < num; ++i)
        {
            if(xi <= x[i])
            {
                float rel = (xi - x[i - 1]) / (x[i] - x[i - 1]);
                ans = y[i - 1] * H0(rel) + y[i] * H0(rel - 1) + dy[i - 1] * H1(rel) + dy[i] * H1(rel - 1);
                return;
            };
        }
    }
    else if(xi < x[i])
    {
        --i;
        for(; i >  -1; --i)
        {
            if(xi >= x[i])
            {
                float rel = (xi - x[i]) / (x[i] - x[i + 1]);
                ans = y[i] * H0(rel) + y[i + 1] * H0(rel - 1) + dy[i] * H1(rel) + dy[i + 1] * H1(rel - 1);
                return;
            };
        }
    }
    else
    {
        ans = y[i];
    }  
}

//vector
void Hermite(float * &x, float * &y, float * &xi, float * &ans)
{
    
}
