#include "Exponential.h"

std::vector<float> Exponential::pdf(const std::vector<float>& X)
{
    std::vector<float>Y(X.size());

    for(size_t i = 0; i < X.size(); ++i)
    {
        Y[i] = Y[i] <= 0 ? 0 : lamada * exp(-lamada * X[i]);
    }

    return Y;
}

std::vector<float> Exponential::cdf(const std::vector<float>& X)
{
    std::vector<float>Y = X;

    for(size_t i = 0; i < X.size(); ++i)
    {
        Y[i] = Y[i] < 0 ? 0 : (1 - exp(-lamada * X[i]));
    }

    return Y;
}
