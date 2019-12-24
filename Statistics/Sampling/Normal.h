//Box - Muller Method
//Single number
#include "Sampling.h"

template<typename T>
class Normal
{
private:
    T mu, sigma;

public:
    Normal<T>(const T& mu, const T& sigma);    
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