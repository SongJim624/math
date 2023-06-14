#include <list>
#include "configuration.hpp"
#include <vector>


size_t Individual::dimensions = 0;
size_t Individual::scales = 0;
size_t Individual::constraints = 0;

int dominate(size_t dimension, double* lhs, double* rhs)
{
	std::array<size_t, 3> counts{ 0, 0, 0 };

	auto compare = [](double lhs, double rhs, T tol = 1e-6) {
		return std::abs(lhs - rhs) < tol ? 1 : (lhs > rhs ? 0 : 2);
	};

	for (size_t i = 0; i < dimension; ++i) {
		counts[compare(lhs[i], rhs[i])]++;
	}

	return (counts[1] == dimension) ? 0 : ((counts[0] == 0) ? 1 : ((counts[2] == 0) ? -1 : 0));
}

//non dominated compare
int operator < (Individual& lhs, Individual& rhs)
{
	//need to further consider the multi non-linear constraints
	int status = Individual::constraints == 0 ? 0 : dominate(1, &lhs.voilations[0], &rhs.voilations[0]);

    return status != 0 ? status :
        dominate(Individual::dimensions, &lhs.objectives[0], &rhs.objectives[0]);
}

Individual::Individual(const Vector<T>& decision) : decisions(decision)
{
	objectives.resize(Individual::dimensions, 0);
	voilations.resize(Individual::constraints, 0);
}