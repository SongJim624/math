#include <time.h>
#include "Sampling.h"

template<typename T>
std::vector<T> Accept_Reject(void* f, const size_t& N, void* g, const T& M)
{
    std::vector<T> X(N);
    srand(time(0));

    size_t i = 0;
    while(i < N)
    {
        T rng = rand() / (T)RAND_MAX;
        T x = sample(g);

        if(rng < f(x) / (M * g(x)))
        {
            X[i] = x;
            ++i;
        }
    }

    return X;
}

template<typename T>
std::vector<T> Envelope_Accept_Reject(void* f, const size_t& N,  void*g, void *gl, const T& M)
{
    std::vector<T> X(N);
    srand(time(0));

    size_t i = 0;
    while(i < N)
    {
        T rng = rand() / (T)RAND_MAX;
        T x = sample(g);

        if(rng < gl(x) / (M * g(x)))
        {
            X[i] = x;
            ++i;
        }
        else if(rng < f(x) / (M * g(x)))
        {
            X[i] = x;
        }
    }

    return X;
}