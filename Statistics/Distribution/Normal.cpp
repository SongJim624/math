#include "Normal.h"

std::vector<float> Normal::pdf(const std::vector<float>& X)
{
    std::vector<float> Y(X.size());

    for(size_t i = 0; i < X.size(); ++i)
    {
        Y[i] = exp(-0.5 * pow((X[i] - mu) / sigma, 2)) / (sqrt(2.0 * PI) * sigma);
    }

	return Y;
}

std::vector<float> Normal::cdf(const std::vector<float>& X)
{
	std::vector<float> Y(X.size());

	for (size_t i = 0; i < X.size(); ++i)
	{
		Y[i] = exp(-0.5 * pow((X[i] - mu) / sigma, 2)) / (sqrt(2.0 * PI) * sigma);
	}

	return Y;
}