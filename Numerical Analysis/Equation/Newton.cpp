#include "Equation.h"

void Equation::Newton(pf &f, pf &df, float &x, const float &Tol = 0.000001)
{
    float y = f(x);
    while(y > Tol)
    {
        x = x - y / df(x);
        y = f(x);
    }
}