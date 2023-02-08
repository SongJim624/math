#include "math.h"
#include "kernel density estimation via diffusion.h"

T Uniform(const T& x)
{
	return abs(x) < 1 ? 0.5 : 0;
};

template<typename T>
T Triangle(const T& x)
{
	return abs(x) < 1 ? 1 - abs(x) : 0;
};

template<typename T>
T Epanechnikov(const T& x)
{
	return abs(x) < 1 ? 0.75 * (1.0 - x * x) : 0;
};

template<typename T>
T Quartic(const T& x)
{
	return abs(x) < 1 ? 0.9375 * (1 - x * x) * (1 - x * x)
		: 0;
};

template<typename T>
T Triweight(const T& x)
{
	return abs(x) < 1 ? 1.09375 * pow((1.0 - x * x), 3)
		: 0;
};

template<typename T>
T Tricube(const T& x)
{
	return abs(x) < 1 ? 70 / 81 * pow((1 - x * x * abs(x)), 3)
		: 0;
};

template<typename T>
T Cosine(const T& x)
{
	return abs(x) < 1 ?  0.25 * PI  * cos(0.5 * PI* x)
		: 0;
};

//No constraint for x
template<typename T>
T Gaussian(const T& x)
{
	return exp(-0.5 * x * x) / sqrt(2.0 * PI);
};

template<typename T>
T Sigmod(const T& x)
{
	return 2.0 / PI / (exp(x) + exp(-x));
};

template<typename T>
T Logistic(const T& x)
{
	return 1.0 / (exp(x) + 2.0 + exp(-x));
};

template<typename T>
T Silverman(const T& x)
{
	return 0.5 * exp(-abs(x) / sqrt(2.0))*sin(abs(x) / sqrt(2.0) + PI / 4.0);
};

void KDE::SetKernel(const std::string& function)
{

}