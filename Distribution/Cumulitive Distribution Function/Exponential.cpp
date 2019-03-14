void Exponential(const float &lamada, const float &x ,float & cdf)
{
    cdf = 1 - exp(-lamada * x);
}