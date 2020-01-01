#include "Sampling.h"

//Metropolis
std::vector<float> Metropolis(Distribution &dist, const size_t& N)
{
    srand(time(NULL));
    std::vector<float> X(N);

    float alpha = 0;

    size_t i =  0;
    while(i < N)
    {
        float u1 = rand() / (float)RAND_MAX;
        float u2 = rand() / (float)RAND_MAX;

        alpha = min(pai(j) * q(i, j) / (pai(i) * q(j, i)), 1);
        
        if(u2 < alpha)
        {
            X[i] = 
            ++i;
        }
    }

    return X;
}


//Metropolis-Hasting
std::vector<float> Metropolis_Hasting(Distribution &dist, const size_t& N)
{
//p proposal
//f distribution to be sampled.
    srand(time(NULL));

    std::vector<float>res(N);

    for(size_t i = 0; i < N;++i)
    {
        float u = rand() / (float)RAND_MAX;

        float X_next = p.sample();

        float alpha = min(dist.pdf(X_next) / f(X), 1);

        if(u < alpha)
        {
            res[i] = X_next;
        }
        else
        {
            res[i] = X;
        }
    }
    return res;
}