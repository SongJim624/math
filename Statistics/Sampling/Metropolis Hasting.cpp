#include "Metropolis Hasting.h"

std::vector<float> Metropolis_Hasting::sampling(const size_t& N, Distribution *dist)
{
//p proposal
//f distribution to be sampled.

    std::vector<float>res(N);

    std::vector<float> X(1);
    X[0] = X0;

    for(size_t i = 0; i < N;++i)
    {
        float u = Rand();

        proposal->ChangeParameter(X[0]);
        std::vector<float> X_next = proposal->sampling(1);
        

        float alpha = fminf(dist->pdf(X_next)[0] / dist->pdf(X)[0], 1);

        if(u < alpha)
        {
            res[i] = X_next[0];
        }
        else
        {
            res[i] = X[0];
        }
    }

    return res;
}

_declspec (dllexport) Sampling* _stdcall Metropolis_Hasting(const float& X0)
{
    return Metropolis_Hasting::Construct(X0);
}