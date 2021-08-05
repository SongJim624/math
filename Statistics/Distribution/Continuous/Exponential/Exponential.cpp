#include "Exponential.h"
#define Location SJML::Statistics::Distribution::Continuous::Exponential

Location::Exponential(const float& lambda)
    : lambda(lambda)
{}

Location::~Exponential()
{}

void Location::density(float * X, float * Y, long size)
{
    cblas_scopy(size, Y, 1, X, 1);
    cblas_sscal(size, -lambda, Y, 1);
    vsExp(size, Y, Y);
    cblas_sscal(size, lambda, Y, 1);

    for(long i = 0; i < size; ++i)
    {
        if (X[i] < 0)
            Y[i] = 0;
    }
}

void Location::distribution(float * X, float * Y, long size)
{
    cblas_scopy(size, Y, 1, X, 1);

    cblas_sscal(size, -lambda, Y, 1);
    vsExp(size, Y, Y);

    for(long i = 0; i < size; ++i)
    {
        Y[i] = 1 - Y[i];
    }
}