#include <time.h>
#include "Sampling.h"

void Accept_Reject(Distribution& proposal, float * samples, long N, void* g, const float& M)
{
    float r = rand() / (float)RAND_MAX;
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