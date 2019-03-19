#pragma once
#include <math.h>
#include <iostream>

typedef float(*pf)(float);

class Equation
{
public:
    void Bisection(pf &function, float &a, float &b,
        float &x, const float &Tol = 0.000001);
    void Newton(pf &f, pf &df, float &x, const float &Tol = 0.000001);
    void Steffensen(pf &f, float &x, const float &Tol = 0.000001);
}