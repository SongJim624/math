#include "Metropolis Hasting.h"

template<typename T>
std::vector<T> Metropolis_Hasting(Distribution<T>& dist, const size_t& N)
{
//here a gaussian proposal is used.    
//p proposal
//f distribution to be sampled.

    std::vector<T>res(N);

    T X0 = 0;

    for(size_t i = 0; i < N;++i)
    {
        T u = rand() / (T) RAND_MAX;
        X_next = proposal->sampling();
        
        T alpha = min(dist->pdf(X_next) / dist->pdf(X), 1);
        res[i] = u < alpha ? X_next : X;
    }

    return res;
}