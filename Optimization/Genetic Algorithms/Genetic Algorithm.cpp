#include "Genetic Algorithms.h"

bool* Individual::integer = nullptr;
float* Individual::upper = nullptr;
float* Individual::lower = nullptr;

long Individual::decision_size = 0;
long Individual::objective_size = 0;

Individual::Individual()
	: decisions(new float[decision_size]), objectives(new float[objective_size]), voilate(0)
{
	for (long i = 0; i < decision_size; ++i)
	{
		decisions[i] = ((float)rand() / RAND_MAX) * (upper[i] - lower[i]) + lower[i];

		if (integer[i])
		{
			decisions[i] = int(decisions[i]);
		}
	}
}
Individual::~Individual()
{
	delete[] decisions;
	delete[] objectives;
}