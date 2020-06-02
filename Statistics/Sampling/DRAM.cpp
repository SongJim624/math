#include "Sampling.h"
#include <limits>
//Using the Gaussian As the Proposal Distribution
//Using the Box-Muller Method to sample from the Gaussian Distribution

template<typename T> 
T Delay_Rejection()
{
    size_t stage = 10;

    for(size_t i  = 0; i < stage; ++i)
    {
        T Y = gaussian_inv();

        if()


    }


}

template<typename T>
std::vector<T> DRAM(Distribution<T>& dist, const size_t& N)
{
    const T sd = 2.4* 2.4;

//Pre-process

//MCMC body of the sampling
    
}