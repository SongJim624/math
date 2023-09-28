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
#include <vector>

#include "../../../basic/math.h"
#include "../evolutionary.h"

/*
 *  article information :
 *
 */

#ifndef _MATH_OPTIMIZATION_SPARSEEA_
#define _MATH_OPTIMIZATION_SAPRSEEA_
using Pointer = std::unique_ptr<double[], decltype(&math::free<double>)>;

Pointer create(size_t length)
{
	return Pointer(math::allocate<double>(length), math::free<double>);
}

class Individual
{
private:
	Pointer data_;
public:
	double *decisions, *objectives, *voilations, *masks;

public:
	Individual(size_t scale, size_t dimension, size_t constraint);
	~Individual();
};

//	implemented in sparseea.cpp
void evaluation(size_t scale, size_t dimension, math::Optimizor::Objective& function, Individual& individual);

class Reference : public Evolutionary::Selector<Individual>
{
private:
	std::shared_ptr<size_t[]> importanaces_;

private:
	size_t dimension_, scale_, constraint_, selection_;
	Pointer<double> ideal_, interception_;

//	simplified reference plain
	std::list<Pointer<double>> points_;
	std::map<double*, std::pair<size_t, std::list<Individual>>> associations_;

private:
	void dispense(size_t needed, Series& elites, Series& cirticals);

	virtual std::list<Series> sort(Series&& population) const;
	virtual std::pair<Series, Series> select(Series&& population);

public:
	Reference(math::Optimizor::Configuration& configuration);
	virtual ~Reference() {}
};

class Reproducor : public GeneticAlgorithm::Reproducor<Individual>
{
private:
	std::shared_ptr<std::map<size_t, std::list<size_t>>> importances_;
	size_t scale_, dimension_;
	double cross_, mutation_, threshold_;
	Pointer<double> upper_, lower_, integer_;
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
	Reproducor(std::shared_ptr<std::map<size_t, std::list<size_t>>> importances, math::Optimizor::Configuration& configuration);
	virtual ~Reproducor() {}
};

class Population : public GeneticAlgorithm::Population, public math::Optimizor::Result
{
private:
	size_t scale_, dimension_, constraint_;
	std::list<Pointer<double>>population_;
	std::list<Pointer<size_t>> masks_;
	std::list<Individual> individuals_;
	std::shared_ptr<std::map<size_t, std::list<size_t>>> importances_;

private:
	std::unique_ptr<GeneticAlgorithm::Selector<Individual>> selector_;
	std::unique_ptr<GeneticAlgorithm::Reproducor<Individual>> reproducor_;

private:
	void importances(double* randoms, math::Optimizor::Objective& function);

private:
	virtual void evolve(size_t generation);
	virtual void write(const char * file);

public:
	Population(math::Optimizor::Configuration& configuration);
	virtual ~Population() {}
};

class SparseEA : public Evolutionary::Evolutionary
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

	virtual ~SparseEA() {}
};
#endif //!_MATH_OPTIMIZATION_UNSGA_