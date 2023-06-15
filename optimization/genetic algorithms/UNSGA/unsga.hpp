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
namespace math = Plain;
using namespace math;

template<typename T, class allocator>
class UNSGA : public Optimizor<T, allocator>
{
private:
	class Population;
	std::unique_ptr<Population> population_;

public:
	virtual const Optimizor<T, allocator>::Result* Optimize(Optimizor<T, allocator>::Configuration* configuration)
	{
		population_ = std::make_unique<Population>(configuration);

		for (size_t i = 0; i < std::get<size_t>(*configuration)["maximum"]; ++i)
		{
			population_->Evolve();
		}

		return reinterpret_cast<Optimizor<T, allocator>::Result*>(population_.get());
	}
};

template<typename T, class allocator>
class UNSGA<T, allocator>::Population : public Optimizor<T, allocator>::Result
{
private:
    class Individual;
    class Reference;
    class Reproducor;
	using Series = std::list<Individual*>;
	using Layer = std::list<std::list<Individual*>>;

private:
    Optimizor<T, allocator>::Configuration* configuration_;

	std::list<std::unique_ptr<Individual>> individuals_;
    std::unique_ptr<Reference> selector_;
    std::unique_ptr<Reproducor> reproducer_;

private:
	std::list<std::list<Individual*>> sort();

private:
	virtual void Write(const char *) const;
	virtual std::vector<std::vector<T, allocator>> decisions() const;
	virtual std::vector<std::vector<T, allocator>> objectives() const;

public:
    void Evolve();

	Population(Optimizor<T, allocator>::Configuration* configuration);
};

template<typename T, class allocator>
class UNSGA<T, allocator>::Population::Individual
{
public:
    std::vector<T, allocator> decisions, objectives, voilations;
	Individual(const std::vector<T, allocator>& decision, size_t dimension, size_t constriant);
};

template<typename T, class allocator>
class UNSGA<T, allocator>::Population::Reference
{
private:
    size_t dimension_;
    class Point;
	std::list<std::unique_ptr<Point>> points_;

private:
	void normalize(Series& elites, Series& cirticals);

public:
    Reference(Optimizor<T, allocator>::Configuration* configuration);
    Series select(Layer layers);
};

template<typename T, class allocator>
class UNSGA<T, allocator>::Population::Reference::Point
{
private:
    std::vector<T, allocator> location_;

public:
    size_t count;
    std::list<Individual*> associated;

public:
    T distance(const std::vector<T, allocator>& point);
    Point(const std::vector<T, allocator>& location);
};

template<typename T, class allocator>
class UNSGA<T, allocator>::Population::Reproducor
{
private:
	T cross_, mutation_, threshold_;
	std::vector<T, allocator> uppers_, lowers_, integers_;

	std::random_device device_;
	std::mt19937_64 generator_;
	std::uniform_real_distribution<T> uniform_;

private:
	void check(Individual& individual);
    void cross(std::array<const Individual*, 2> parents, std::array<Individual*, 2> children);
    void mutate(Individual& individual);

public:
	Reproducor(Optimizor<T, allocator>::Configuration* configuration);
	Series reproduce(std::pair<Series, Series> population);
};
#endif //!_MATH_OPTIMIZATION_UNSGA_