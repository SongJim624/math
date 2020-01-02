#include "Gamma.h"

std::vector<float> Gamma::pdf(const std::vector<float>& X)
{
    std::vector<float> Y(X.size());

    for(size_t i = 0; i < Y.size(); ++i)
    {
        Y[i] = pow(beta, alpha) * pow(X[i], alpha - 1) *
            exp(-beta * X[i]) / tgamma(alpha);
    }

    return Y;
}

std::vector<float> Gamma::cdf(const std::vector<float>& X)
{
    std::vector<float> Y(X.size());

    return Y;
}