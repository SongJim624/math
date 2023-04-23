
#include "../../optimizer.h"
#include <time.h>
#include <list>
#include <map>
#include <algorithm>
#include <memory>
#include <array>
#include <cassert>
#include <mkl.h>
#include "math.hpp"

#ifndef _MATH_OPTIMIZATION_UNSGA_CONFIGURATION_
#define _MATH_OPTIMIZATION_UNSGA_CONFIGURATION_
template<typename T>
struct Configuration
{
public:
	size_t dimensions, scales, constraints;
	const T* uppers, * lowers;
	const bool* integers;
	Optimization::Objective<T>* objective;
	Optimization::Constraint<T>* constraint;

public:
	size_t maximum, division, population;

	std::vector<std::vector <T, Allocator<T>>> initialization;

	T cross, mutation, threshold;
	VSLStreamStatePtr stream;

public:
	void Update(Optimization::Configuration<T>* configuration);

public:
	Configuration(Optimization::Configuration<T>* configuration);
	~Configuration();
};

template<typename T>
Configuration<T>::Configuration(Optimization::Configuration<T>* configuration)
	: dimensions(configuration->dimensions()), scales(configuration->scales()), constraints(configuration->constraints()),
	uppers(configuration->uppers()), lowers(configuration->lowers()), integers(configuration->integers()),
	objective(configuration->objective.get()), constraint(configuration->constraint.get())
{
	const auto& config = *configuration;

	maximum = std::get<size_t>(config["maximum"]);

	cross = std::get<T>(config["cross"]);
	mutation = std::get<T>(config["mutation"]);

	vslNewStream(&stream, VSL_BRNG_MCG31, 1);
}

template<typename T>
Configuration<T>::~Configuration()
{
	vslDeleteStream(&stream);
}

template<typename T>
void Configuration<T>::Update(Optimization::Configuration<T>* configuration) {
}
#endif //!_MATH_OPTIMIZATION_UNSGA_CONFIGURATION_

