#include "Normal.h"

Sampling* _stdcall Box_Muller(const float& mu, const float& sigma, const size_t& type)
{
    return Box_Muller::Construct(mu, sigma, type);
}

std::vector<float> sampling1(const size_t& N, const float& mu, const float& sigma)
{
    std::vector<float> X(N);

    size_t i = 0;
    
    while(true)
    {
        float u1 = Rand();
        float u2 = Rand();

        X[i] = sqrt(-2.0 * logf(u1)) * cosf(u2) * sigma + mu;
        ++i;

        if(i == N)
        {
            break;
        }

        X[i] = sqrt(-2.0 * logf(u1)) * sinf(u2) * sigma + mu;
        ++i;

        if(i == N)
        {
            break;
        }
    }

    return X;
}

std::vector<float> sampling2(const size_t& N, const float& mu, const float& sigma)
{
    std::vector<float> X(N);

    size_t i = 0;
    
    while(true)
    {
        float u1 = rand();
        float u2 = rand();

        X[i] = sqrt(-2.0 * logf(u1)) * cosf(u2) * sigma + mu;
        ++i;

        if(i == N)
        {
            break;
        }

        X[i] = sqrt(-2.0 * logf(u1)) * sinf(u2) * sigma + mu;
        ++i;

        if(i == N)
        {
            break;
        }
    }

    return X;
}