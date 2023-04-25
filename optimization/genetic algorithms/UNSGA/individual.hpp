#include <list>
#include "configuration.hpp"
#include <vector>

#ifndef _MATH_OPTIMIZATION_UNSGA_INDIVIDUAL_
#define _MATH_OPTIMIZATION_UNSGA_INDIVIDUAL_
template<typename T>
class Individual
{
public:
	static size_t dimensions, scales, constraints;

public:
	Vector<T> decisions, objectives, voilations;

	std::list<Individual<T>*> dominates;
	size_t dominated;

public:
    Individual(const Vector<T>& decisions);
};

template<typename T>
size_t Individual<T>::dimensions = 0;

template<typename T>
size_t Individual<T>::scales = 0;

template<typename T>
size_t Individual<T>::constraints = 0;


template<typename T>
int dominate(size_t dimension, T* lhs, T* rhs) {
	std::array<size_t, 3> counts{ 0, 0, 0 };

	auto compare = [](T lhs, T rhs, T tol = 1e-6) {
		return std::abs(lhs - rhs) < tol ? 1 : (lhs > rhs ? 0 : 2);
	};

	for (size_t i = 0; i < dimension; ++i) {
		counts[compare(lhs[i], rhs[i])]++;
	}

	return (counts[1] == dimension) ? 0 : ((counts[0] == 0) ? 1 : ((counts[2] == 0) ? -1 : 0));
}

//seems not necessary, just use dimension = 1 in the previous case
//dominate relations of the voilation
//template<typename T>
//int dominate(T lhs, T rhs) {
//	return lhs == 0 ? (rhs == 0 ? 0 : 1) : (rhs == 0 ? -1 : int(compare(lhs, rhs)) - 1);
//}

//non dominated compare
template<typename T>
int operator < (Individual<T>& lhs, Individual<T>& rhs) {
	//need to further consider the multi non-linear constraints
	int status = Individual<T>::constraints == 0 ? 0 : dominate(1, &lhs.voilations[0], &rhs.voilations[0]);

    return status != 0 ? status :
        dominate(Individual<T>::dimensions, &lhs.objectives[0], &rhs.objectives[0]);
}

template<typename T>
Individual<T>::Individual(const Vector<T>& decision) : decisions(decision), dominates({}), dominated(0) {
	objectives.resize(Individual<T>::dimensions, 0);
	voilations.resize(Individual<T>::constraints, 0);
}

template<typename T>
using Series = std::list<Individual<T>*>;

template<typename T>
using Layer = std::list<std::list<Individual<T>*>>;
#endif //!_MATH_OPTIMIZATION_UNSGA_INDIVIDUAL_