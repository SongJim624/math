#include <list>
#include <map>
#include <memory>

#include "../optimizor.h"

#ifndef _math_optimization_evolutionary_framework_
#define _math_optimization_evolutionary_framework_
namespace Evolutionary
{
    template<typename T>
    concept Individual = requires(T individual)
    {
        individual.decisions;
        individual.objectives;
        individual.voilations;
    };

    template<Individual T>
    class Selector
    {
    public:
        virtual std::list<std::list<T*>> sort(const std::list<T*>& population) const = 0;
        virtual std::pair<std::list<T*>, std::list<T*>> select(const std::list<T*>& population) = 0;
        virtual ~Selector() {}
    };

    template<Individual T>
    class Reproducor
    {
    protected:
        virtual void cross(const T& father, const T& mother, T& son, T& daughter) = 0;
        virtual void mutate(T& individual) = 0;
        virtual void check(T& individual) = 0;

    public:
        virtual std::list<T*> reproduce(std::pair<std::list<T*>, std::list<T*>>&& population) = 0;
        virtual ~Reproducor() {}
    };

//  Population is the class who is responsible for different method of initialization
    template<Individual T>
    class Population
    {
    public:
        virtual Selector<T>& selector() = 0;
        virtual Reproducor<T>& reproducor() = 0;
        virtual ~Population() {}
    };

//   the evolutionary algorithms only involves one specie
    class Evolutionary : public math::Optimizor, public math::Optimizor::Result
    {
    protected:
        virtual void evolve(size_t generation) = 0;

        virtual Optimizor::Result& optimize(Optimizor::Configuration& configuration) = 0;

        virtual void write(const char*, char mode) = 0;
        virtual std::list<std::shared_ptr<const double[]>> results() = 0;
       
    public:
        virtual ~Evolutionary() {}
    };

//  the co-evolutionary algorithm involves multi species, mainly aim at large scale problems
    class Coevolutionary : public math::Optimizor, public math::Optimizor::Result
    {
    protected:
        virtual void coevole(size_t generation) = 0;

    public:
        virtual ~Coevolutionary() {}
    };
}
#endif //! _math_optimiation_evolutionray_framework_