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

#include "../../../basic/math.h"
#include "../genetic algorithm.h"

#ifndef _MATH_OPTIMIZATION_UNSGA_
#define _MATH_OPTIMIZATION_UNSGA_
using Individual = double *;
using Series = GeneticAlgorithm::Series<Individual>;

class Reference : public GeneticAlgorithm::Selector<Individual>
{
private:
	size_t dimension_, scale_, constraint_, selection_;
	std::unique_ptr<double[], decltype(&math::free)> ideal_, interception_;

//	simplified reference plain
	std::list<std::unique_ptr<double[], decltype(&math::free)>> points_;
	std::map<double*, std::pair<size_t, std::list<Individual>>> associations_;

private:
	void dispense(size_t needed, Series& elites, Series& cirticals);

	virtual std::list<Series> sort(Series&& population) const;
	virtual std::pair<Series, Series> select(Series&& population);

public:
	Reference(const math::Optimizor::Configuration& configuration);
	virtual ~Reference() {}
};

class Reproducor : public GeneticAlgorithm::Reproducor<Individual>
{
private:
	size_t scale_, dimension_;
	double cross_, mutation_, threshold_;
	std::unique_ptr<double[], decltype(&math::free)> upper_, lower_, integer_;
	math::Optimizor::Objective *function_;

private:
	std::mt19937_64 generator_;
	std::uniform_real_distribution<double> uniform_;

private:
	virtual void check(Individual individuals);
	virtual void cross(const Individual parents[2], Individual children[2]);
	virtual void mutate(Individual individua);

private:
	virtual Series reproduce(std::pair<Series, Series>&& population);

public:
	Reproducor(math::Optimizor::Configuration& configuration);
	virtual ~Reproducor() {}
};

class Population : public GeneticAlgorithm::Population, public math::Optimizor::Result
{
private:
	std::unique_ptr<GeneticAlgorithm::Selector<Individual>> selector_;
	std::unique_ptr<GeneticAlgorithm::Reproducor<Individual>> reproducor_;

private:
	size_t scale_, dimension_, constraint_;
	std::list<std::unique_ptr<double[], decltype(&math::free)>> population_;
	std::list<double*> individuals_;

private:
	virtual void evolve(size_t generation);
	virtual void write(const char * file);
public:
	Population(math::Optimizor::Configuration& configuration);
	virtual ~Population() {}
};

class UNSGA : public GeneticAlgorithm::Optimizor
{
private:
	std::unique_ptr<GeneticAlgorithm::Population> population_;

public:
	virtual Optimizor::Result& optimize(math::Optimizor::Configuration& configuration)
	{
		population_ = std::make_unique<Population>(configuration);
		population_->evolve(std::get<size_t>(configuration["maximum"]));
		return *dynamic_cast<Population*>(population_.get());
	}

	virtual ~UNSGA() {}
};
#endif //!_MATH_OPTIMIZATION_UNSGA_