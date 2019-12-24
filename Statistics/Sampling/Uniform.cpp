//This for the continuous distribution
#include "Uniform.h"

template<typename T>
Uniform<T>::Uniform(const T& a, const T& b)
: a(a), b(b)
{}

template<typename T>
std::vector<T> Uniform<T>::pdf(const std::vector<T>& X)
{
    std::vector<T> Y = X;

    for(size_t i = 0; i < X.size();++i)
    {
        Y[i] = 1 / (this->b - this>a);
    }
}

void Uniform(const float &a = 0.0, const float &b = 1.0, float &rng)
{
    rng = (rand() / (float)RAND_MAX) * (b - a) + a;
}


void Uniform(const float &a = 0.0, const float &b = 1.0, float &rng)
{
    rng = (rand() / (float)RAND_MAX) * (this->b - this->a) + this->a;
}