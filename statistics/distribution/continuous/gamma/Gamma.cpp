#include "Normal.h"
#define Location SJML::Statistics::Distribution::Continuous::Gamma

Location::Gamma(const float& alpha, const float& beta)
	: alpha(alpha), beta(beta)
{}

Location::~Gamma()
{}

void Location::density(float * X, float * Y, long size)
{
    cblas_scopy(size, Y, 1, X, 1);
	cblas_sscal(size, -beta, Y, 1);
    vsExp(size, Y, Y);

    float * temp = new float[size];
    for(long i = 0;  i < size; ++i)
    {
        temp[i] = alpha - 1;
    }
    vsPow(size, X, temp, temp);
    vsMul(size, temp, Y, Y);

	cblas_sscal(size, pow(beta, alpha) / tgammaf(alpha), Y, 1);
    delete[] temp;
	return;
}

void Location::distribution(float* X, float* Y, long size)
{
    float * temp = new float[size];
    for(long i = 0; i< size; ++i)
    {
        temp[i] = alpha;
    }

    cblas_scopy(size,  Y, 1, X, 1);
    cblas_sscal(size, beta, Y, 1);
    vslgamma(size, temp, Y, Y);
    cblas_sscal(size, 1 / tgammaf(alpha), Y, 1);
    delete[] temp;
	return;
}