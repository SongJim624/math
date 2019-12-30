#include "Sampling.h"

//Metropolis
template<typename T>
std::vector<T> Metropolis(void* f, const size_t& N)
{
    std::vector<T> X(N);

    srand(time(0));

    size_t i = 0;
    T alpha = 0;

    while(i < N)
    {
        T u1 = (T)rand() / RAND_MAX;
        T u2 = (T)rand() / RAND_MAX;


        alpha = min(pai(j) * q(i, j) / (pai(i) * q(j, i)), 1);
        
        if(u2 < alpha)
        {
            X[i] = 
            ++i;
        }
    }


    return Y;
}


//Metropolis-Hasting
template<typename T>
std::vector<T> Metropolis_Hasting(void* f , const size_t& N)
{
//p proposal
//f distribution to be sampled.
    std::vector<T> res(N);

    srand(time(NULL));

    T u = rand() / (T)RAND_MAX;

    T X_next = p.sample();

    T alpha = min(f(X_next) / f(X), 1);

    if(u < alpha)
    {
        res[i] = X_next;
    }
    else
    {
        res[i] = X;
    }
}