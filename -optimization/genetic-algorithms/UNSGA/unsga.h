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
#include <cassert>

class UNSGA : public Genetic
{
public:
	class Individual;
	class Population;
	class Result;
	struct Configuration;

	template<class T>
	class Allocator
	{
	public:
		using value_type = T;
		Allocator() = default;

		template<class other>
		Allocator(const Allocator<other>&) {};

		void deallocate(T* pointer, const size_t length)
		{
			mkl_free(pointer);
		}

		T* allocate(size_t length)
		{
			return static_cast<T*>(mkl_malloc(length * sizeof(T), 64));
		}
	};

public:
	typedef std::vector<float, Allocator<float>> vector;

private:
	class Reference;
	class Reproducor;

private:
	std::shared_ptr<Configuration> configuration_;
	std::unique_ptr<Population> population_;
	std::unique_ptr<Reproducor> reproducor_;
	std::unique_ptr<Reference> reference_;
	std::shared_ptr<Optimizer::Result> results_;

private:
	void check(Objective* objective);

public:
	virtual std::shared_ptr<Optimizer::Result> Optimize(Objective * information);

public:
	UNSGA(const Configuration& configuration);
	~UNSGA();
};

class UNSGA::Population
{
private:
	std::shared_ptr<Configuration> configuration_;
	std::vector<std::unique_ptr<Individual>> individuals_;

	std::list<Individual*> population_;
private:
	virtual void fitness(Individual*);

public:
	std::list<std::list<Individual*>> sort();
	void Update(std::list<Individual*>& population);

public:
	Population(std::shared_ptr<Configuration> configuration);
	~Population();
};

class UNSGA::Individual : public Genetic::Individual
{
private:
	std::shared_ptr<Configuration> configuration_;

public:
	virtual int operator < (const Genetic::Individual& individual) const;

public:
	Individual(std::shared_ptr<Configuration> configuration);
	Individual(std::shared_ptr<Configuration> configuration, const std::vector<float>& initial);
	~Individual();

public:
	float penalty;
	size_t dominated;
	std::list<Individual*> dominates;
};

class UNSGA::Result : public Optimizer::Result
{
public:
	virtual std::vector<std::vector<float>> decisions() const;
	virtual std::vector<std::vector<float>> objectives() const;

	virtual void Write(const char* path) const;
};

class UNSGA::Reference
{
private:
typedef std::pair<std::map<Individual*, UNSGA::vector>, std::map<Individual*, UNSGA::vector>> Cost;
	class Point;

private:
	std::shared_ptr<Configuration> configuration_;
	std::vector<std::unique_ptr<Point>> storage_;

private:
	std::list<Point*> points_;

private:
	UNSGA::vector  Ideal(const std::list<Individual*>& individuals);
	UNSGA::vector Interception(const std::list<Individual*>& individuals, const UNSGA::vector& ideal);
	Cost Normalize(const std::list<Individual*> solution, const std::list<Individual*>& critical);
	void Associate(const Cost& costs);
	void Dispense(size_t needed, std::list<Individual*>& solution, std::list<Individual*>& critical);

public:
	void Niche(size_t needed, std::list<Individual*>& solution, std::list<Individual*>& critical);

public:
	Reference(std::shared_ptr<Configuration> configuration);
	~Reference();

public:
	size_t size() const;
	std::pair<std::list<Individual*>, std::list<Individual*>> Select(std::list<std::list<Individual*>>& layers);
};

class UNSGA::Reference::Point
{
private:
	std::shared_ptr<Configuration> configuration_;
	UNSGA::vector location_;

public:
	size_t count;
	std::list<Individual*> associated;

public:
	Point(const float* location, std::shared_ptr<Configuration> configuration);
	~Point();

	//perpendicular distance for a point to the reference line
	float distance(const UNSGA::vector& point) const;
};

class UNSGA::Reproducor
{
private:
	std::shared_ptr<Configuration> configuration_;

private:
	void Cross(const Individual& father, const Individual& mother, Individual& son, Individual& daughter);
	void Mutate(Individual& individual);
	void check(Individual& individual);

public:
	Reproducor(std::shared_ptr<Configuration> configuration);
	~Reproducor();

public:
	//worse individuals in the children set would be replaced by children
	std::list<Individual*> Reproduce(std::pair<std::list<Individual*>, std::list<Individual*>>& population);
};

struct UNSGA::Configuration
{
	//algorithm configuration
	size_t maximum, division, population;
	std::vector<std::vector <float>> initialization;

	//for the genetic operation
	float cross, mutation, threshold;
	VSLStreamStatePtr stream;

	//objective information
	size_t dimension, scale;
	Objective* objective;
};
#endif // !_Mathematical_Tools_Optimization_Genetic_Algorithms_UNSGA_
