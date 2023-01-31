#ifndef _Mathematical_Tools_Optimization_Genetic_Algorithms_UNSGA_
#define _Mathematical_Tools_Optimization_Genetic_Algorithms_UNSGA_

#include "../genetic algorithms.h"
#include <time.h>
#include <list>
#include <map>
#include <algorithm>
#include <memory>
#include <array>
#include <mkl.h>

class UNSGA : public Genetic
{
public:
	class Individual;
	class Population;

private:
	struct Configuration;
	class Reference;
	class Reproducor;

private:
	std::shared_ptr<Configuration> configuration_;
	std::unique_ptr<Population> population_;
	std::unique_ptr<Reproducor> reproducor_;

private:
	virtual void Select(Genetic::Population& population);
	virtual void Reproduce(Genetic::Population& population);

private:
//	void Dominate(size_t size, Individual* lhs, Individual* rhs);
//	std::list<std::list<Individual*>> Sort(std::list<Individual*>& population);
//	void Select(Reference& plain, std::list<Individual*>& solution, std::list<Individual*>& population);

public:
	virtual std::vector<std::vector<float>> Optimize(Information * information);

public:
	UNSGA(const Configuration& configuration);
	~UNSGA();
};

class UNSGA::Population : public Genetic::Population
{
private:
	std::list<Individual*> population_;

private:
	std::list<std::list<Individual*>> Sort(std::list<Individual*>& population);
};


class UNSGA::Individual : public Genetic::Individual
{
private:
	std::shared_ptr<Configuration> configuration;

public:
	virtual int operator < (const Genetic::Individual& individual);

public:
	Individual(std::shared_ptr<Configuration> configuration);
	~Individual();

public:
	float voilation;
	size_t dominated;
	std::list<Individual*> dominates;
};

class UNSGA::Reference
{
private:
typedef std::pair<std::map<Population::Individual*, float *>, std::map<Population::Individual*, float *>> Cost;
	class Point;

private:
	size_t objectives_;
	std::list<Point *> points_;

private:
//the functions are not that rigid because the memory applied in Ideal and Interception are released in Normalize.
	float * Ideal(const std::list<Population::Individual*>& individuals);
	float * Interception(const std::list<Population::Individual*>& individuals, const float * ideal, const float * costs);
	Cost Normalize(const std::list<Population::Individual*> solution, const std::list<Population::Individual*>& critical, float * costs);
	void Associate(const Cost& costs);
	void Dispense(size_t needed, std::list<Individual*>& solution, std::list<Individual*>& critical);

public:
	Reference(size_t objective, size_t division);
	~Reference();

public:
	void Niche(size_t needed, std::list<Individual*>& solution, std::list<Individual*>& critical);
};

class UNSGA::Reference::Point
{
private:
	std::weak_ptr<size_t> dimension;
	float * location_;

public:
	size_t count;
	std::list<Individual*> associated;

public:
	Point(const float * location);
	~Point();

	//perpendicular distance for a point to the reference line
	float distance(const float * point) const;
};

class UNSGA::Reproducor
{
private:
	std::shared_ptr<VSLStreamStatePtr> stream;
	size_t dimension_;

private:
	float cross_, mutation_, threshold_;

private:
	void Cross(const Individual& father, const Individual& mother, Individual& son, Individual& daughter);
	void Mutate(Individual& individual);
	void check(Individual& individual);

public:
	virtual void Reproduce(std::list<Individual*>& solution, std::list<Individual*>& population);

public:
	static void Initialize(VSLStreamStatePtr stream);
	static void Finalize();
};

struct UNSGA::Configuration
{
	size_t maximum, dimension, division;
	VSLStreamStatePtr stream;
};
#endif // !_Mathematical_Tools_Optimization_Genetic_Algorithms_UNSGA_
