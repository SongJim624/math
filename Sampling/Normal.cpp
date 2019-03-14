//Box - Muller Method
//Single number
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