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
{

}

//non dominated compare
int UNSGA::Individual::operator < (const Genetic::Individual& individual)
{
	int status = compare(voilation, dynamic_cast<const Individual*>(&individual)->voilation);

	if (status == 0)
	{
		status = dominate(configuration->dimension, objectives, individual.objectives);
	}
}
