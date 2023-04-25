
#include "../../optimizer.h"
#include <time.h>
#include <list>
#include <map>
#include <algorithm>
#include <memory>
#include <array>
#include <cassert>
#include <mkl.h>
#include "D:\\Windows\\Documents\\GitHub\\mathematical-tools\\basic\\eigen like\\vector.hpp"

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

	std::vector<Vector<T>> initialization;

	T cross, mutation, threshold;

public:
	Configuration(Optimization::Configuration<T>* configuration);
	~Configuration();
};

template<typename T>
Configuration<T>::Configuration(Optimization::Configuration<T>* configuration) {
	dimensions = configuration->dimensions();
	scales = configuration->scales();
	constraints = configuration->constraints();

	uppers = configuration->uppers();
	lowers = configuration->lowers();
	integers = configuration->integers();

	objective = configuration->objective.get();
	constraint = configuration->constraint.get();

	const auto& config = *configuration;

	maximum = std::get<size_t>(config["maximum"]);
	division = std::get<size_t>(config["division"]);
	population = std::get<size_t>(config["population"]);

	cross = std::get<T>(config["cross"]);
	mutation = std::get<T>(config["mutation"]);
}

template<typename T>
Configuration<T>::~Configuration(){
}
#endif //!_MATH_OPTIMIZATION_UNSGA_CONFIGURATION_

