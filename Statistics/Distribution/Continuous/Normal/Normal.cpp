#include "Normal.h"
#define Location SJML::Statistics::Distribution::Continuous::Normal

Location::Normal(const float& mu, const float& sigma)
	:pi(3.1415926), mu(mu), sigma(sigma)
{}

Location::~Normal()
{}

void Location::density(float * X, float * Y, long size)
{
	for(long i = 0; i <  size; ++i)
	{
		Y[i] = -mu;
	}

	vsAdd(size, X, Y, Y);
	vsMul(size, Y, Y, Y);
	cblas_sscal(size, -0.5f * powf(sigma, -2.0f), Y, 1);
	vsExp(size, Y, Y);
	cblas_sscal(size, 1 / (sqrtf(2 * pi) * sigma), Y, 1);
	return;
}

void Location::distribution(float* X, float* Y, long size)
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