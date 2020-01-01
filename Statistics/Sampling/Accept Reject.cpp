#include <time.h>
#include "Sampling.h"

double Accept_Reject(void* f, const size_t& N, void* g, const double& M)
{
    double rng = rand() / (double)RAND_MAX;
    double x = sample(g);

    if(rng < f(x) / (M * g(x)))
    {
        X[i] = x;
        ++i;
    }

    return rng;
}

double Envelope_Accept_Reject(void* f, void*g, void *gl, const double& M)
{
    double rng = rand() / (double)RAND_MAX;
    double x = sample(g);

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