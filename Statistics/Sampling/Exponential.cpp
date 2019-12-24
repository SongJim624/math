#include "Exponential.h"

template<typename T>
Exponential<T>::Exponential(const T& x)
: lamada(x)
{}

template<typename T>
std::vector<T> Exponential<T>::pdf(const std::vector<T>& X)
{
    std::vector<T>Y = X;

    for(size_t i = 0; i < X.size(); ++i)
    {
        Y[i] = Y[i] <= 0 ? 0 : this->lamada * exp(-this->lamada * X[i]);
    }

    return Y;
}

template<typename T>
std::vector<T> Exponential<T>::cdf(const std::vector<T>& X)
{
    std::vector<T>Y = X;

    for(size_t i = 0; i < X.size(); ++i)
    {
        Y[i] = Y[i] < 0 ? 0 : (1 - exp(-this->lamada * X[i]));
    }

    return Y;
}

inline void rand_Exponential(const float &la = 1.0, float &rng)
{
    rng -logf((float)rand() / (float)RAND_MAX) / la; 
}

void rand_Exponential(const float &la = 1.0, size_t num, float * &rng)
{
    for(size_t i = 0; i < num; ++i)
    {
        rng[i] -logf((float)rand() / (float)RAND_MAX) / la; 
    }
}