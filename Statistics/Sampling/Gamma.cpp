#include "Gamma.h"

template<typename T>
Gamma<T>::Gamma(const T& alpha, const T& beta)
: alpha(alpha), beta(beta)
{

}

template<typename T>
std::vector<T> Gamma<T>::pdf(const std::vector<T>& X)
{
    std::vector<T> Y = X;

    for(size_t i = 0; i < Y.size(); ++i)
    {
        Y[i] = pow(this->beta, this->alpha) * pow(X[i], alpha - 1) *
            exp(-this->beta * X[i]) / gamma(alpha);
    }

    return Y;
}

template<typename T>
std::vector<T> Gamma<T>::cdf(const std::vector<T>& X)
{
    
}