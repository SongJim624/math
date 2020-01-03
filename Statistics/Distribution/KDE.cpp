#include "KDE.h"

std::vector<float> KDE::pdf(const std::vector<float> &X)
{
	std::vector<float> Y(X.size());

	for(size_t i = 0; i < X.size(); ++i)
	{
		float Density = 0;

		for (int j = 0; j < h.size(); ++j)
		{
			Density += kernel((X[i] - xData[j]) / h[j]) / h[j];
		};

		Y[i] = Density / h.size();
	}
	return Y;
};

std::vector<float> KDE::cdf(const std::vector<float>& X)
{
	std::vector<float> Y(X.size());

	return Y;
}
float Uniform(const float& x)
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

float Triangle(const float& x)
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

float Epanechnikov(const float& x)
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

float Quartic(const float& x)
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

float Triweight(const float& x)
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

float Tricube(const float& x)
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

float Cosine(const float& x)
{
	if ((x >= 1.0) || (x <= -1.0))
	{
		return 0.0;
	}
	else
	{
		return 0.25 * PI  * cos(0.5 * PI* x);
	};
};


//No constraint for x
float Gaussian(const float& x)
{
	return exp(-0.5 * x * x) / sqrt(2.0 * PI);
};

float Sigmod(const float& x)
{
	return 2.0 / PI / (exp(x) + exp(-x));
};

float Logistic(const float& x)
{
	return 1.0 / (exp(x) + 2.0 + exp(-x));
};

float Silverman(const float& x)
{
	return 0.5 * exp(-abs(x) / sqrt(2.0))*sin(abs(x) / sqrt(2.0) + PI / 4.0);
};
