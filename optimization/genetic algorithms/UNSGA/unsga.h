#include <list>
#include <map>
#include <string>
#include <time.h>
#include <algorithm>
#include <memory>
#include <array>
#include <random>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <variant>
#include <exception>
#include <numeric>
#include <utility>
#include <ranges>

#include "../../../math library.h"
#include "../evolutionary.h"

#ifndef _MATH_OPTIMIZATION_UNSGA_
#define _MATH_OPTIMIZATION_UNSGA_
class Individual
{
private:
	math::pointer<double> data_;

public:
	double *decisions, *objectives, *voilations;

public:
	Individual(size_t scale, size_t dimension, size_t constraint);
};

class Reference : public Evolutionary::Selector<Individual>
{
private:
	size_t dimension_, scale_, constraint_, selection_;
	math::pointer<double> ideal_, interception_;

//	simplified reference plain
	std::list<std::tuple<math::pointer<double>, size_t, std::list<Individual*>>> associations_;

private:
	void dispense(size_t needed, std::list<Individual*>& elites, std::list<Individual*>& cirticals);

	virtual std::list<std::list<Individual*>> sort(const std::list<Individual*>& population) const;
	virtual std::pair<std::list<Individual*>, std::list<Individual*>> select(const std::list<Individual*>& population);

public:
	Reference(const math::Optimizor::Configuration& configuration);
	virtual ~Reference() {}
};

class Reproducor : public Evolutionary::Reproducor<Individual>
{
private:
	size_t scale_, dimension_;
	double cross_, mutation_, threshold_;
	math::pointer<double> upper_, lower_, integer_;
	math::Optimizor::Objective *function_;

private:
	std::mt19937_64 generator_;
	std::uniform_real_distribution<double> uniform_;

private:
	virtual void check(Individual& individuals);
	virtual void cross(const Individual& father, const Individual& mother, Individual& son, Individual& daughter);
	virtual void mutate(Individual& individua);

private:
	virtual std::list<Individual*> reproduce(std::pair<std::list<Individual*>, std::list<Individual*>>&& population);

public:
	Reproducor(math::Optimizor::Configuration& configuration);
	virtual ~Reproducor() {}
};

class Population : public Evolutionary::Population<Individual>
{
private:
	std::unique_ptr<Reference> selector_;
	std::unique_ptr<Reproducor> reproducor_;

public:
	virtual Evolutionary::Selector<Individual>& selector();
	virtual Evolutionary::Reproducor<Individual>& reproducor();

public:
	size_t scale, dimension, constraint;
	std::list<Individual*> individuals;

public:
	Population(math::Optimizor::Configuration& configuration);
	virtual ~Population();
};

class UNSGA : public Evolutionary::Evolutionary
{
private:
	std::unique_ptr<Population> population_;
	std::list<std::shared_ptr<const double[]>> elites_;

protected:
	virtual void write(const char * filepath, char mode);
	virtual std::list<std::shared_ptr<const double[]>> results();

	virtual void evolve(size_t generation);
	virtual math::Optimizor::Result& optimize(math::Optimizor::Configuration& configuration);

public:
	virtual ~UNSGA() {}
};

#ifdef __WIN32__
	#define EXPORT __declspec(dllexport)
#else
	#define EXPORT __attribute__((visibility("default")))
#endif

extern "C" EXPORT void* __cdecl create();
#endif //!_MATH_OPTIMIZATION_UNSGA_