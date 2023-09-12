#include <time.h>
#include <list>
#include <map>
#include <algorithm>
#include <memory>
#include <array>
#include <random>
#include <cassert>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <variant>
#include <exception>
#include <numeric>

#include "../../../basic/math.h"
#include "../../optimizor.h"

//This is not included in the math namespace
//it is just an implementation and should be load via a library
//and instanced by a factory
#ifndef _MATH_OPTIMIZATION_UNSGA_
#define _MATH_OPTIMIZATION_UNSGA_
class UNSGA : public math::Optimizor
{
private:
	class Population;
	std::unique_ptr<Population> population_;

public:
	virtual const Optimizor::Result& Optimize(Optimizor::Configuration& configuration);
};

class UNSGA::Population : public Optimizor::Result
{
private:
//	class Individual;
	class Reference;
	class Reproducor;

private:
	const size_t dimension_, scale_, constraint_;
	using Individual = double*;
//	Series individuals_;

	std::list<double*> individuals_;
	std::unique_ptr<Reference> selector_;
	std::unique_ptr<Reproducor> reproducer_;

private:
	std::list<std::list<double*>> sort(std::list<double*> population) const;
	virtual void Write(const char *) const;

public:
	const Optimizor::Result& evolve(size_t generation);

	Population(Optimizor::Configuration& configuration);
	~Population();
};

class UNSGA::Population::Reference
{
private:
	const size_t scale_, dimension_, selection_;
	double* ideal_, * interception_;

	class Point;
	std::list<std::unique_ptr<Point>> points_;

private:
	void attach(const double * individual);
	void associate(double * inddividual, double * cost);

	void dispense(size_t needed, std::list<double*>& elites, std::list<double*>& cirticals);

public:
	Reference(const Optimizor::Configuration& configuration);
	// the operator is the select function
	std::pair<std::list<double*>, std::list<double*>> operator () (std::list<std::list<double*>> layers);
};

class UNSGA::Population::Reference::Point
{
private:
	const size_t dimension_;
	double* location_;

public:
	size_t count;
	std::list<double*> associated;

public:
	double distance(const double * point) const;
	Point(size_t dimension, const double * location);
};

class UNSGA::Population::Reproducor
{
private:
	const size_t scale_, dimension_;
	double *uppers_, *lowers_, *integers_;
	math::Optimizor::Objective& function_;
	double cross_, mutation_, threshold_;

	std::mt19937_64 generator_;
	std::uniform_real_distribution<double> uniform_;

private:
	void check(double * individual);
	void cross(std::array<const double*, 2> parents, std::array<double*, 2> children);
	void mutate(double * individual);

public:
	Reproducor(math::Optimizor::Configuration& configuration);
	std::list<double*> operator() (std::pair<std::list<double*>, std::list<double*>> population);
};
#endif //!_MATH_OPTIMIZATION_UNSGA_