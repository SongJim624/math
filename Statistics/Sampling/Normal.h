//Box - Muller Method
//Single number
#include "Sampling.h"

template<typename T>
class Normal : public PDF<T>, public CDF<T>, public Sample<T>
{
private:
    T mu, sigma;

private:
    std::vector<T> pdf(const std::vector<T>& X);
    std::vector<T> cdf(const std::vector<T>& X);

public:
    Normal<T>(const T& mu, const T& sigma)
    : mu(mu), sigma(sigma)
    {};    
};

inline void Normal(const float &mu, const float &sigma, float &rng)
{
    rng = sqrt(-2.0 * logf(rand() / (float)RAND_MAX)) *
        cosf(rand() / (float)RAND_MAX) * sigma + mu;
}

//Vector
void Normal(const float &mu, const float &sigma, size_t num, float * &rngs)
{
    for(size_t i = 0; i < num; ++i)
    {
        rngs[i] = sqrt(-2.0 * logf(rand() / (float)RAND_MAX)) *
            cosf(rand() / (float)RAND_MAX) * sigma + mu;        
    }
}