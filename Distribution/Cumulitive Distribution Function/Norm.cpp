void Norm(const float &mu, const float &sigma, const float &x, float &cdf)
{
    cdf = 0.5 * (1.0 + erf(0.5 * sqrt(2) * (x - mu) / sigma));
}