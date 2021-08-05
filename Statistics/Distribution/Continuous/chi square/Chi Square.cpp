#include "Normal.h"
#define Location SJML::Statistics::Distribution::Continuous::ChiSquare

Location::ChiSquare(const long& dF)
	: k(dF))
{}

Location::~ChiSquare()
{}

void Location::density(float * X, float * Y, long size)
{
    cblas_scopy(size, Y, 1, X, 1);
	cblas_sscal(size, -0.5f, Y, 1);
    vsExp(size, Y, Y);

    float * temp = new float[size];
    for(long i = 0;  i < size; ++i)
    {
        temp[i] = 0.5f * k - 1;
    }
    vsPow(size, X, temp, temp);
    vsMul(size, temp, Y, Y);

	cblas_sscal(size, pow(2.0f, -0.5f * k) / tgammaf(0.5f * k), Y, 1);
    delete[] temp;
	return;
}

void Location::distribution(float* X, float* Y, long size)
{
    float * temp = new float[size];
    for(long i = 0; i< size; ++i)
    {
        temp[i] = k;
    }

    cblas_scopy(size,  Y, 1, X, 1);
    cblas_sscal(size, 0.5, Y, 1);
    cblas_sscal(size, 0.5, temp, 1);
    vslgamma(size, temp, Y, Y);
    cblas_sscal(size, 1 / tgammaf(0.5f * k), Y, 1);
    delete[] temp;
	return;
}