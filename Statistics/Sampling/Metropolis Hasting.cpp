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
std::vector<T> Metropolis_Hasting(void* pdf, const size_t& N)
{

}