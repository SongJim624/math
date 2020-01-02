#include <time.h>
#include "Sampling.h"

float Accept_Reject(void* f, const size_t& N, void* g, const float& M)
{
    float rng = rand() / (float)RAND_MAX;
    float x = sample(g);

    if(rng < f(x) / (M * g(x)))
    {
        X[i] = x;
        ++i;
    }

    return rng;
}

float Envelope_Accept_Reject(void* f, void*g, void *gl, const float& M)
{
    float rng = rand() / (float)RAND_MAX;
    float x = sample(g);

    if(rng < gl(x) / (M * g(x)))
    {
        X[i] = x;
        ++i;
    }
    else if(rng < f(x) / (M * g(x)))
    {
        X[i] = x;
    }

    return X;
}