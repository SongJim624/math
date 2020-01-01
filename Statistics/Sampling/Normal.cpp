#include "Normal.h"

template<typename T>
std::vector<T> Normal<T>::pdf(const std::vector<T>& X)
{
    std::vector<T> Y(X.size());

    for(size_t i = 0; i < X.size(); ++i)
    {
        Y[i] = exp(-0.5 * pow((X[i] - mu) / sigma, 2) / (sqrt(2.0 * pi) * sigma);
    }
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