#include "Normal.h"

std::vector<float> Box_Muller::sampling(const size_t& N, Distribution* dist)
{

    switch(type)
    {
    case 1:
    {    
        std::vector<float> X = sampling1(N, mu, sigma);
        return X;
    }
    case 2:
    {    
        std::vector<float> X = sampling2(N, mu, sigma);
        return X;
    }
    case 3:
    {    
        std::vector<float> X = sampling3(N, mu, sigma);
        return X;
    }
    default:
    {    
        std::vector<float> X = sampling1(N, mu, sigma);
        return X;
    }
    }
}

void Box_Muller::ChangeParameter(const float& a, const float& b, const float& c, const float& d)
{
    mu = a;

    sigma = (b == 0.0) ? sigma : b;
}

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

std::vector<float> sampling3(const size_t& N, const float& mu, const float& sigma)
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