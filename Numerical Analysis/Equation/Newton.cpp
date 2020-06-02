#include "Equation.h"
constexpr tol 0.000001

template<typename T>
T Newton(pf &f, pf &df, const T &x)
{
    T xn = x + 1;
    while(abs(x - xn) > Tol)
    {
        xn = x - f(x) / df(x);
    }

    return xn;
}