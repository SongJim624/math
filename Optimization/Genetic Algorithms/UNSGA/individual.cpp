#include "unsga.h"

size_t compare(float lhs, float rhs, float tolerance = 1e-9)
{
	return std::abs(lhs - rhs) < tolerance ? 1 : (lhs > rhs ? 0 : 2);
}

//dominate relations of the objectives
int dominate(size_t dimension, const float* lhs, const float* rhs)
{
	std::array<size_t, 3> counts{ 0, 0, 0 };

	//	size_t equal = 0, more = 0, less = 0;

	for (size_t i = 0; i < dimension; ++i)
	{
		counts[compare(lhs[i], rhs[i])]++;
	}

	return (counts[1] == dimension) ? 0 : ((counts[0] == 0) ? 1 : ((counts[2] == 0) ? -1 : 0));
}

//dominate relations of the voilation
int dominate(float lhs, float rhs)
{
	return lhs == 0 ? (rhs == 0 ? 0 : 1) : (rhs == 0 ? -1 : int(compare(lhs, rhs)) - 1);
}

UNSGA::Individual::Individual(std::shared_ptr<Configuration> configuration)
	: penalty(0), dominated(0), dominates({})
{
	decisions = (float*)mkl_malloc(configuration->scale * sizeof(float), 64);
	float * random = (float*)mkl_malloc(configuration->scale * sizeof(float), 64);
	objectives = (float*)mkl_malloc(configuration->dimension * sizeof(float), 64);

	vsRngUniform(VSL_RNG_METHOD_UNIFORM_STD, configuration->stream, configuration->scale, random, 0, 1);
	vsSub(configuration->scale, configuration->objective->upper(), configuration->objective->lower(), decisions);
	vsMul(configuration->scale, random, decisions, decisions);
	vsAdd(configuration->scale, configuration->objective->lower(), decisions, decisions);

	for(size_t i = 0; i < configuration->scale; ++i)
	{
		if (configuration->objective->integer()[i])
		{
			decisions[i] = std::round(decisions[i]);
		}
	}

	mkl_free(random);
	random = nullptr;
}

UNSGA::Individual::Individual(std::shared_ptr<Configuration> configuration, const float * initial)
	: penalty(0), dominated(0), dominates({})
{
	decisions = (float*)mkl_malloc(configuration->scale * sizeof(float), 64);
	objectives = (float*)mkl_malloc(configuration->dimension * sizeof(float), 64);
	cblas_scopy(configuration->scale, initial, 1, decisions, 1);
}

UNSGA::Individual::~Individual()
{
	mkl_free(decisions);
	mkl_free(objectives);
	
	decisions = nullptr;
	objectives = nullptr;
}

//non dominated compare
int UNSGA::Individual::operator < (const Genetic::Individual& individual) const
{
	int status = compare(penalty, dynamic_cast<const Individual*>(&individual)->penalty);

	if (status == 0)
	{
		status = dominate(configuration->dimension, objectives, individual.objectives);
	}
	
	return status;
}
