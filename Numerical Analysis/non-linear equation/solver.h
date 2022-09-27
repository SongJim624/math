#pragma once
#include <math.h>
#include <functional>

class Solver
{
private:
    float tolerance_;

public:
    float Bisection(float left, float right);
    float Newton(float begin);
    float Steffenson(float begin);

public:
    Solver(std::function<float(float)>& function, float tolerance, size_t code);

    float tolerance() const;
    float& tolerance();
};