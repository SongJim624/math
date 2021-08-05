#include "Uniform.h"
#define Location SJML::Statistics::Distribution::Continuous::Uniform

Location::Uniform(const float& a, const float& b)
	: left(a), right(b)
{}

Location::~Uniform()
{}

void Location::density(float * X, float * Y, long size)
{
	for(long i = 0; i <  size; ++i)
	{
		Y[i] = X[i] < left ? 0 : X[i] > right ? 0 : 1;
	}

	cblas_sscal(size, 1 / (right - left), Y, 1);
	return;
}

void Location::distribution(float* X, float* Y, long size)
{
	for(long i = 0; i <  size; ++i)
	{
		Y[i] = X[i] < left ? 0 : fminf(X[i], right) - left;
	}

	cblas_sscal(size, 1 / (right - left), Y, 1);
	return;
}