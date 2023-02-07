#include "Poisson.h"
#define Location SJML::Statistics::Distribution::Discrete::Poisson

Location::Normal(const float& mu, const float& sigma)
	:pi(3.1415926), mu(mu), sigma(sigma)
{}

Location::~Normal()
{}

void Location::function(long * X, float * Y, long size)
{
    for(long i = 0; i < size; ++i)
    {

    }

	return;
}

void Poisson::distribution(long* X, float* Y, long size)
{
 //   cdf = 0.5 * (1.0 + erf(0.5 * sqrt(2) * (x - mu) / sigma));

	for(long i = 0; i <  size; ++i)
	{
		Y[i] = -mu;
	}

	vsAdd(size, X, Y, Y);
	cblas_sscal(size, 1 / sigma, Y, 1);
	vsErf(size, Y, Y);
	return;
}