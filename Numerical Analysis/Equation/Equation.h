#pragma once
#include <math.h>
#include <functional>

class Solver
{
private:
    float tolerance_;
    std::function<float(float)> function_, derivate_;

public:
    float Bisection(float left, float right);
    float Newton(float begin);
    float Steffenson(float begin);

public:
    Solver(std::function<float(float)>&, float);

    float tolerance() const;
    float& tolerance();
};

class MatrixSolver
{
public:
    std::vector<float> Doolittle();
    std::vector<float> Gauss();
    std::vector<float> Jacobi();
};