#include "Equation.h"

void Equation::Bisection(pf &f, float &a, float &b,
    float &x, const float & Tol = 0.000001)
{
    float fa = f(a);
    float fb = f(b);

    if(fa * fb > 0)
    {
        printf("Error : The value of f(a)f(b) is larger than 0 !\n");
        return;
    }

    size_t type = fa > 0 ? 1 : 2;
    
    x = 0.5 * (b - a);
    float y = f(x);

    switch(type)
    {
        case 1:
        {
            while(y > Tol)
            {
                if(y > 0)
                {
                    a = x;
                }
                else
                {
                    b = x;
                }
                
                x = 0.5 * (b - a);
                y = f(x);
            }
            return;
        }
        case 2:
        {
            while(y > Tol)
            {
                if(y > 0)
                {
                    b = x;
                }
                else
                {
                    a = x;
                }
                
                x = 0.5 * (b - a);
                y = f(x);
            }
            return;
        }
    }
}