inline void rand_Exponential(const float &la = 1.0, float &rng)
{
    rng -logf((float)rand() / (float)RAND_MAX) / la; 
}

void rand_Exponential(const float &la = 1.0, size_t num, float * &rng)
{
    for(size_t i = 0; i < num; ++i)
    {
        rng[i] -logf((float)rand() / (float)RAND_MAX) / la; 
    }
}