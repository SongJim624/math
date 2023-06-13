#include "../../optimizor.h"
#include <time.h>
#include <list>
#include <map>
#include <algorithm>
#include <memory>
#include <array>
#include <cassert>

#ifndef _MATH_OPTIMIZATION_UNSGA_
#define _MATH_OPTIMIZATION_UNSGA_
struct Configuration
{
public:
	size_t dimensions, scales, constraints;
	const double* uppers, * lowers;
	const bool* integers;
	Optimizor::Objective* objective;
	Optimizor::Constraint* constraint;

public:
	size_t maximum, division, population;
//	std::vector<Vector<T>> initialization;
	double cross, mutation, threshold;

public:
	Configuration(Optimizor::Configuration* configuration);
	~Configuration();
};

class Individual
{
public:
	static size_t dimensions, scales, constraints;

public:
	Vector<T> decisions, objectives, voilations;

public:
    Individual(const Vector<T>& decisions);
};
int operator < (Individual& lhs, Individual& rhs);

class Result : public Optimizor::Result
{
private:
	Series<T> elites_;

public:
	virtual void Write(const char* filename) const;
	virtual std::vector<std::vector<T>> objectives() const;
	virtual std::vector<std::vector<T>> decisions() const;

public:
	Result(const Series<T>& elites);
};


class UNSGA : public Optimizor
{
private:
	class Population;

private:
	std::unique_ptr<Configuration> configuration_;
	std::unique_ptr<Population> population_;
	std::unique_ptr<Optimizor::Result> results_;

public:
	UNSGA();
	virtual const Optimizor::Result* Optimize(Optimizor::Configuration* configuration);
};
#endif //!_MATH_OPTIMIZATION_UNSGA_