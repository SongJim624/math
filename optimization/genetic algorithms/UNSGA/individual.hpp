#include <list>
#include "configuration.hpp"

#ifndef _MATH_OPTIMIZATION_UNSGA_INDIVIDUAL_
#define _MATH_OPTIMIZATION_UNSGA_INDIVIDUAL_
template<typename T>
class Individual
{
public:
	std::vector<T, Allocator<T>> decisions, objectives, voilations;
	std::list<const Individual*> dominates;
	size_t dominated;

public:
    Individual(const std::vector<T, Allocator<T>>& decisions) : decisions(decisions) {}
};

template<typename T>
size_t compare(T lhs, T rhs, T tolerance = 1e-9)
{
	return std::abs(lhs - rhs) < tolerance ? 1 : (lhs > rhs ? 0 : 2);
}

//dominate relations of the objectives
template<typename T>
int dominate(const std::vector<T, Allocator<T>>& lhs, const std::vector<T, Allocator<T>>& rhs)
{
	size_t dimension = lhs.size();
	std::array<size_t, 3> counts{ 0, 0, 0 };

	auto compare = [](T lhs, T rhs, T tol) {
		return std::abs(lhs - rhs) < tol ? 1 : (lhs > rhs ? 0 : 2);
	};

	for (size_t i = 0; i < dimension; ++i)
	{
		counts[compare(lhs[i], rhs[i])]++;
	}

	return (counts[1] == dimension) ? 0 : ((counts[0] == 0) ? 1 : ((counts[2] == 0) ? -1 : 0));
}

//dominate relations of the voilation
int dominate(float lhs, float rhs) {
	return lhs == 0 ? (rhs == 0 ? 0 : 1) : (rhs == 0 ? -1 : int(compare(lhs, rhs)) - 1);
}

//non dominated compare
template<typename T>
int operator < (const Individual<T>& lhs, const Individual<T>& rhs) {
	int status = dominate(*lhs.voilations, *rhs.voilations);

    return status != 0 ? status :
        dominate(lhs.objectives, rhs.individual.objectives);
}

template<typename T>
using Series = std::list<Individual<T>*>;

template<typename T>
using Layer = std::list<std::list<Individual<T>*>>;
#endif //!_MATH_OPTIMIZATION_UNSGA_INDIVIDUAL_