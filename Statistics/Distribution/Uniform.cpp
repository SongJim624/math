//This for the continuous distribution
#include "Uniform.h"

std::vector<float> Uniform::pdf(const std::vector<float>& X)
{
    std::vector<float> Y(X.size());

    for(size_t i = 0; i < X.size();++i)
    {
        Y[i] = 1.0 / (b - a);
    }

    return Y;
}

std::vector<float> Uniform::cdf(const std::vector<float>& X)
{
    std::vector<float> Y(X.size());

    for(size_t i = 0; i < X.size();++i)
    {
        Y[i] = (X[i] - a) / (b - a);
    }

    return Y;
}