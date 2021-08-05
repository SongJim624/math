#include "Metropolis Hasting.h"

void Metropolis_Hasting(Distribution& dist, float * samples, long N)
{
//here a gaussian proposal is used.    
//p proposal
//f distribution to be sampled.

    float X0 = 0;

    for(size_t i = 0; i < N;++i)
    {
        float u = rand() / (T) RAND_MAX;
        X_next = proposal->sampling();
        
        float alpha = min(dist->pdf(X_next) / dist->pdf(X), 1);
        res[i] = u < alpha ? X_next : X;
    }

    return res;
}