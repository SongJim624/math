#include "Equation.h"
constexpr tol 0.000001

template<typename T>
T Steffensen(pf &f, const T& x)
{
    T xn = x + 1;
    while(abs(xn - x) > tol)
    {
        xn = x - pow(f(x), 2) / (f(x + f(x)) - f(x));
    }
}