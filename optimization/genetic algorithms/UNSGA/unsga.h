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
	class Individual;
	class Reference;
	class Reproducor;
	using Series = std::list<Individual*>;

private:
	Optimizor::Objective& function_;
	Series individuals_;
	std::unique_ptr<Reference> selector_;
	std::unique_ptr<Reproducor> reproducer_;

private:
	std::list<Series> sort();

private:
	virtual void Write(const char *) const;

public:
	void Evolve();
	Population(Optimizor::Configuration& configuration);
	~Population();
};

class UNSGA::Population::Individual
{
public:
	double *decisions, *objectives, *voilations;

public:
	Individual(size_t scale, size_t dimension, size_t constriant);
	~Individual();
};

class UNSGA::Population::Reference
{
private:
	const size_t dimension_;

	class Point;
	std::list<Point*> points_;

private:
	void normalize(Series& elites, Series& cirticals);

public:
	Reference(const Optimizor::Configuration& configuration);
	Series select(std::list<Series> layers);
};

class UNSGA::Population::Reference::Point
{
private:
	const size_t dimension_;
	double* location_;

public:
	size_t count;
	std::list<Individual*> associated;

public:
	double distance(const double * point) const;
	Point(size_t dimension, const double * location);
};

class UNSGA::Population::Reproducor
{
private:
	const size_t dimension, scale;
	double *uppers_, *lowers_, *integers_;
	double cross_, mutation_, threshold_;

	std::random_device device_;
	std::mt19937_64 generator_;
	std::uniform_real_distribution<double> uniform_;

private:
	void check(Individual& individual);
	void cross(std::array<const Individual*, 2> parents, std::array<Individual*, 2> children);
	void mutate(Individual& individual);

public:
	Reproducor(const math::Optimizor::Configuration& configuration);
	Series reproduce(std::pair<Series, Series> population);
};
#endif //!_MATH_OPTIMIZATION_UNSGA_