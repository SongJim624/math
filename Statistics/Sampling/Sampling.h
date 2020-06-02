#pragma once
#include <time.h>
#include "../Distribution/Distribution.h"

template<typename T>
T Mean(const std::vector<T>& A)
{
    T res = 0;
    for(size_t i = 0; i < A.size(); ++i)
    {
        res += A[i];
    }

    return res / A.size();
}

template<typename T>
T Variance(const std::vector<T>& A)
{
    T E = Mean(A);
    T res = 0;
    for(size_t i = 0; i < A.size(); ++i)
    {
        res += (A[i] - E) * (A[i] - E);
    }

    return res / A.size();
}

//Basic Inverse transform Sampling
template<typename T>
T Box_Muller(const T& mu = 0.0, const T& sigma = 1.0)
{    
    T u1 = rand() / (T) RAND_MAX;
    T u2 = rand() / (T) RAND_MAX;

    return rand() / (T) RAND_MAX > 0.5 ? 
        sqrt(-2.0 * log(u1)) * cos(2 * 3.1415926 * u2) * sigma + mu :
        sqrt(-2.0 * log(u1)) * sin(2 * 3.1415926 * u2) * sigma + mu;
}