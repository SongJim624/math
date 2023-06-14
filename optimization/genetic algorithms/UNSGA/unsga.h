#include "../../optimizor.h"
#include <list>
#include <map>
#include <algorithm>
#include <memory>
#include <array>
#include <cassert>
#include <random>

#ifndef _MATH_OPTIMIZATION_UNSGA_
#define _MATH_OPTIMIZATION_UNSGA_

#if 1
#define allocator std::allocator<double>;
#endif

class Configuration
{
public:
	size_t dimensions, scales, constraints;
	std::vector<double, allocator> uppers, lowers, integers;
	Optimizor::Objective* objective;
	Optimizor::Constraint* constraint;

public:
	size_t maximum, division, population;
	std::vector<std::vector<double, allocator>> initialization;
	double cross, mutation, threshold;

public:
	Configuration(Optimizor::Configuration* configuration);
};

class Individual
{
public:
	static size_t dimensions, scales, constraints;

public:
	double *decisions, *objectives, *voilations;
	Individual(const std::vector<double, allocator>& decision);
};

class Point
{
private:
	double* location_;

public:
	size_t count;
	std::list<Individual*> associated;

public:
	//perpendicular distance for a point to the reference line
	double distance(const std::vector<double>& point) const;
    Point(const std::vector<double>& point);
};

class Reference
{
private:
	using Series = std::list<Individual*>;

private:
    size_t dimension_;
	std::list<std::unique_ptr<Point>> points_;
	std::vector<double> ideals_, interceptions_;

private:
	// archievement scalar function
    double Scale(size_t dimension, const std::vector<double>& objectives);
	void  Ideal(const Series& individuals);
	void Interception(const Series& individuals);

	void Normalize(Series individuals);
	void Associate(Series solution, Series& critical);
	void Dispense(size_t needed, Series& solution, Series& critical);

public:
    std::pair<std::list<Individual*>, std::list<Individual*>> Select(std::list<std::list<Individual*>> layers);
    Reference(Configuration* configuration);
};

class Reproducor
{
private:
    size_t scale_;
    std::vector<double> uppers_, lowers_, integers_;
    double cross_, mutation_, threshold_;

    std::random_device device_;
    std::mt19937_64 generator_;
    std::uniform_real_distribution<double> uniform_;

private:
    void Cross(std::array<const Individual*, 2> parents, std::array<Individual*, 2> children);
    void Mutate(Individual* individual);
    void check(Individual& individual);

public:
    Reproducor(Configuration* configuration);
	//worse individuals in the children set would be replaced by children
    std::list<Individual*> Reproduce(std::pair<std::list<Individual*>, std::list<Individual*>> population);
};

class Population
{
private:
    friend class Result;
    Optimizor::Objective * objective_;
    Optimizor::Constraint * constraint_;

    std::unique_ptr<Reference> selector_;
    std::unique_ptr<Reproducor> reproducer_;

	std::list<std::unique_ptr<Individual>> individuals_;

private:
	std::list<std::list<Individual*>> sort();
    void fitness(Individual& individual);

public:
    void Evolve();
    std::list<Individual*> Elite();

public:
	Population(Configuration* configuration);
    ~Population();
};

class Result : public Optimizor::Result
{
private:
	std::list<Individual*> elites_;

public:
	virtual void Write(const char* filename) const;
	virtual std::vector<std::vector<double>> objectives() const;
	virtual std::vector<std::vector<double>> decisions() const;

public:
	Result(const std::list<Individual*>& elites);
};

class UNSGA : public Optimizor
{
private:
//	std::unique_ptr<Configuration> configuration_;
	std::unique_ptr<Population> population_;
	std::unique_ptr<Optimizor::Result> results_;

public:
	UNSGA();
	virtual const Optimizor::Result* Optimize(Optimizor::Configuration* configuration);
};
#endif //!_MATH_OPTIMIZATION_UNSGA_