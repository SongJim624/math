#include "KDE.h"
#define PI 3.1415926

//The method used in this function is variable 
template<typename T>
KDE<T>::KDE(float *h, float *xData, float (*K) (float ), int n)
{
	float Density = 0;

	for (int i = 0; i < n; i++)
	{
		Density = Density + K((x - xData[i]) / h[i]) / h[i];
	};

	return Density / n ;

};

template<typename T>
std::vector<T> KDE<T>::pdf(const std::vector<T> &X)
{
	float Density = 0;

	for (int i = 0; i < n; i++)
	{
		Density = Density + K((x - xData[i]) / h[i]) / h[i];
	};

	return Density / n ;

};

template<typename T>
T KDE<T>::Uniform(T x)
{
	if ((x >= 1)||(x <= -1))
	{
		return 0.0;
	}
	else
	{
		return 0.5;
	};
};

template<typename T>
T KDE<T>::Triangle(T x)
{
	if ((x >= 1.0) || (x <= -1.0))
	{
		return 0.0;
	}
	else
	{
		return 1 - abs(x);
	};
};

template<typename T>
T KDE<T>::Epanechnikov(T x)
{
	if ((x >= 1.0) || (x <= -1.0))
	{
		return 0.0;
	}
	else
	{
		return 3.0 * (1.0 - pow(x, 2.0)) / 4.0;
	};
};

template<typename T>
T KDE<T>::Quartic(T x)
{
	if ((x >= 1.0) || (x <= -1.0))
	{
		return 0.0;
	}
	else
	{
		return 15.0 / 16.0 * pow((1.0 - pow(x, 2.0)), 2.0);
	};
};

template<typename T>
T KDE<T>::Triweight(T x)
{
	if ((x >= 1.0) || (x <= -1.0))
	{
		return 0.0;
	}
	else
	{
		return 35.0 / 32.0 * pow((1.0 - x * x), 3.0);
	};
};

template<typename T>
T KDE<T>::Tricube(T x)
{
	if ((x >= 1.0) || (x <= -1.0))
	{
		return 0.0;
	}
	else
	{
		return 70 / 81 * pow((1 - x * x * abs(x)), 3);
	};
};

template<typename T>
T KDE<T>::Cosine(T x)
{
	if ((x >= 1.0) || (x <= -1.0))
	{
		return 0.0;
	}
	else
	{
		return PI / 4.0 * cos(0.5 * PI* x);
	};
};


//No constraint for x
template<typename T>
T KDE<T>::Gaussian(T x)
{
	return exp(-0.5 * pow(x, 2.0)) / sqrt(2.0 * PI);
};

template<typename T>
T KDE<T>::Sigmod(T x)
{
	return 2 / PI / (exp(x) + exp(-x));
};

template<typename T>
T KDE<T>::Logistic(T x)
{
	return 1.0 / (exp(x) + 2.0 + exp(-x));
};

template<typename T>
T KDE<T>::Silverman(T x)
{
	return 0.5 * exp(-abs(x) / sqrt(2.0))*sin(abs(x) / sqrt(2.0) + PI / 4.0);
};