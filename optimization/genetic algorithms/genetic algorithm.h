#include <list>
#include <map>
#include <memory>

#include "../optimizor.h"

#ifndef _MATH_OPTIMIZATION_GENETIC_ALGORITHM_
#define _MATH_OPTIMIZATION_GENETIC_ALGORITHM_
namespace GeneticAlgorithm
{
    using Individual = double *;
    using Series = std::list<Individual>;

    class Selector
    {
    public:
        virtual std::list<Series> sort(Series&& population) = 0;
        virtual std::pair<Series, Series> select(Series&& population) = 0;
        virtual ~Selector() {}
    };

    class Reproducor
    {
    protected:
        virtual void cross(const Individual parents[2], Individual children[2]) = 0;
        virtual void mutate(Individual individual) = 0;
        virtual void check(Individual individual) = 0;

    public:
        virtual Series reproduce(std::pair<Series, Series>&& population) = 0;
        virtual ~Reproducor() {}
    };

    class Population
    {
    public:
        virtual void evolve(size_t generation) = 0;
        virtual ~Population() {}
    };

    class Optimizor : public math::Optimizor
    {
    public:
        virtual const Optimizor::Result& optimize(math::Optimizor::Configuration& configuration) = 0;
        virtual ~Optimizor() {}
    };
}
#endif //! _MATH_OPTIMIZATION_GENETIC_ALGORITHM_