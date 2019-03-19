#include "Equation.h"

void Equation::Steffensen(pf &f, float &x, const float &Tol = 0.000001)
{
    float y = f(x);
    while(y > Tol)
    {
        x = x - y * y / (f(x + y) - y);
        y = f(x);
    }
}