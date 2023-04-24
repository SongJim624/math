#include <list>
#include "configuration.hpp"
#include <vector>

#ifndef _MATH_OPTIMIZATION_UNSGA_INDIVIDUAL_
#define _MATH_OPTIMIZATION_UNSGA_INDIVIDUAL_
template<typename T>
class Individual
{
private:
	Vector<T> decisions_, objectives_, voilations_;

public:
	static size_t dimensions, scales, constraints;

public:
	T *decisions, *objectives, *voilations;
	std::list<const Individual*> dominates;
	size_t dominated;

public:
    Individual(const Vector<T>& decisions);
	~Individual();
};

template<typename T>
int dominate(size_t dimension, const T* lhs, const T* rhs) {
	std::array<size_t, 3> counts{ 0, 0, 0 };

	auto compare = [](T lhs, T rhs, T tol) {
		return std::abs(lhs - rhs) < tol ? 1 : (lhs > rhs ? 0 : 2);
	};

	for (size_t i = 0; i < dimension; ++i) {
		counts[compare(lhs[i], rhs[i])]++;
	}

	return (counts[1] == dimension) ? 0 : ((counts[0] == 0) ? 1 : ((counts[2] == 0) ? -1 : 0));
}

//seems not necessary, just use dimension = 1 in the previous case
//dominate relations of the voilation
template<typename T>
int dominate(T lhs, T rhs) {
	return lhs == 0 ? (rhs == 0 ? 0 : 1) : (rhs == 0 ? -1 : int(compare(lhs, rhs)) - 1);
}

//non dominated compare
template<typename T>
int operator < (const Individual<T>& lhs, const Individual<T>& rhs) {
	int status = dominate(1, lhs.voilations, rhs.voilations);

    return status != 0 ? status :
        dominate(Individual<T>::dimensions, lhs.objectives, rhs.individual.objectives);
}

template<typename T>
Indivudial<T>::Individual(const Vector<T>& decision) : decisions_(decision), dominates({}), dominated(0) {
	objectives_.resize(Individual<T>::dimension);
	voilations_.resize(Individual<T>::constraints);

	decisions = &decisions_[0];
	objectives = &objectives_[0];
	voilations = Individual<T>::constriants == 0 ? nullptr : &constraints_[0];
}

template<typename T>
Individual<T>::~Individual() {
	decisions = nullptr;
	objectives = nullptr;
	voilations = nullptr;
}

template<typename T>
using Series = std::list<Individual<T>*>;

template<typename T>
using Layer = std::list<std::list<Individual<T>*>>;
#endif //!_MATH_OPTIMIZATION_UNSGA_INDIVIDUAL_