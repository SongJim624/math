//This for the continuous distribution
#include "Uniform.h"

template<typename T>
std::vector<T> Uniform<T>::pdf(const std::vector<T>& X)
{
    std::vector<T> Y(X.size());

    for(size_t i = 0; i < X.size();++i)
    {
        Y[i] = 1.0 / (b - a);
    }
}

template<typename T>
std::vector<T> Uniform<T>::cdf(const std::vector<T>& X)
{
    std::vector<T> Y(X.size());

    for(size_t i = 0; i < X.size();++i)
    {
        Y[i] = (X[i] - a) / (b - a);
    }
}