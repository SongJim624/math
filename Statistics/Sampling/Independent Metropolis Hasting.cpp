#include "Metropolis Hasting.h"

std::vector<float> Independent_Metropolis_Hasting::sampling(const size_t& N, Distribution* dist)
{
    std::vector<float> X(N);
    std::vector<float> Xt(1);
    Xt[0] = X[0] = X0;

    for(size_t i = 1; i < N; ++i)
    {
        std::vector<float> Yt = proposal->sampling(1);
        float alpha = fminf(dist->pdf(Yt)[0] * g->pdf(Xt)[0] /
            (dist->pdf(Xt)[0] * g->pdf(Yt)[0]), 1);
        
        float u = Rand();
        if(u < alpha)
        {
            X[i] = Yt[0];
        }
        else 
        {
            X[i] = X[i - 1];
        }

        Xt[0] = X[i];
    }

    return X;

}