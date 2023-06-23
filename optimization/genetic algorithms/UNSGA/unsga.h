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

#include "../../../basic/plain math/plain.h"
#include "../../optimizor.h"

#ifndef _MATH_OPTIMIZATION_UNSGA_
#define _MATH_OPTIMIZATION_UNSGA_
using namespace math;

class UNSGA : public Optimizor
{
private:
	class Population;
	std::unique_ptr<Population> population_;

public:
	virtual const Optimizor::Result* Optimize(Optimizor::Configuration* configuration)
	{
		population_ = std::make_unique<Population>(configuration);

		for (size_t i = 0; i < std::get<size_t>((*configuration)["maximum"]); ++i)
		{
			population_->Evolve();
		}

		return reinterpret_cast<Optimizor::Result*>(population_.get());
	}
};

class UNSGA::Population : public Optimizor::Result
{
private:
    class Individual;
    class Reference;
    class Reproducor;
	using Series = std::list<Individual*>;
	using Layer = std::list<std::list<Individual*>>;

private:
    Optimizor::Configuration* configuration_;

	std::list<std::unique_ptr<Individual>> individuals_;
    std::unique_ptr<Reference> selector_;
    std::unique_ptr<Reproducor> reproducer_;

private:
	std::list<std::list<Individual*>> sort();

private:
	virtual void Write(const char *) const;

public:
    void Evolve();
	Population(Optimizor::Configuration* configuration);
};

class UNSGA::Population::Individual
{
public:
    Matrix<double> decisions, objectives, voilations;
	Individual(const std::vector<double>& decision, size_t dimension, size_t constriant);
};

class UNSGA::Population::Reference
{
private:
    size_t dimension_;
    class Point;
	std::list<std::unique_ptr<Point>> points_;

private:
	void normalize(Series& elites, Series& cirticals);

public:
    Reference(Optimizor::Configuration* configuration);
    Series select(Layer layers);
};

class UNSGA::Population::Reference::Point
{
private:
    std::vector<double> location_;

public:
    size_t count;
    std::list<Individual*> associated;

public:
    double distance(const std::vector<double>& point);
    Point(const std::vector<double>& location);
};


class UNSGA::Population::Reproducor
{
private:
	double cross_, mutation_, threshold_;
	Matrix<double> uppers_, lowers_, integers_;

	std::random_device device_;
	std::mt19937_64 generator_;
	std::uniform_real_distribution<double> uniform_;

private:
	void check(Individual& individual);
    void cross(std::array<const Individual*, 2> parents, std::array<Individual*, 2> children);
    void mutate(Individual& individual);

public:
	Reproducor(Optimizor::Configuration* configuration);
	Series reproduce(std::pair<Series, Series> population);
};
#endif //!_MATH_OPTIMIZATION_UNSGA_