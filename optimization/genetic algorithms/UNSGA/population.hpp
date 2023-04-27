#include "individual.hpp"
#include "reproducer.hpp"
#include "reference plain.hpp"
#include "configuration.hpp"

#ifndef _MATH_OPTIMIZATION_UNSGA_POPULATION_
#define _MATH_OPTIMIZATION_UNSGA_POPULATION_
template<typename T>
class Result;

template<typename T>
class Population
{
private:
    friend class Result<T>;

private:
    Optimization::Objective<T> * objective_;
    Optimization::Constraint<T> * constraint_;

    std::unique_ptr<Reference<T>> selector_;
    std::unique_ptr<Reproducor<T>> reproducer_;

	std::vector<std::unique_ptr<Individual<T>>> individuals_;
	Series<T> population_;

private:
	Layer<T> sort();
    void fitness(Individual<T>& individual);

public:
    void Evolve();
    Series<T> Elite();

	Population(Configuration<T>* configuration);
    ~Population();
};

template<typename T>
Population<T>::Population(Configuration<T>* configuration) {
    objective_ = configuration->objective;
    constraint_ = configuration->constraint;
    selector_ = std::make_unique<Reference<T>>(configuration);
    reproducer_ = std::make_unique<Reproducor<T>>(configuration);

    individuals_.resize(configuration->population);
    size_t count = 0;

    for(auto& individual : individuals_) {
        individual = std::make_unique<Individual<T>>(configuration->initialization[count++]);
        fitness(*individual);
        population_.push_back(individual.get());
    }
}

template<typename T>
Population<T>::~Population() {
    objective_ = nullptr;
    constraint_ = nullptr;
}

template<typename T>
void Population<T>::fitness(Individual<T>& individual) {
    (*objective_)(&individual.decisions[0], &individual.objectives[0]);
    (*constraint_)(&individual.decisions[0], &individual.objectives[0], Individual<T>::constraints == 0 ? nullptr : &individual.voilations[0]);
}

template<typename T>
Layer<T> Population<T>::sort() {
    Layer<T> results;

    for (auto individual = population_.begin(); individual != population_.end(); ++individual)
    {
        for (auto later = std::next(individual); later != population_.end(); ++later)
        {
            switch ((**individual) < (**later))
            {
            case 1 :
            {
                (*individual)->dominates.push_back(*later);
                (*later)->dominated++;
                continue;
            }
            case -1 :
            {
                (*later)->dominates.push_back(*individual);
                (*individual)->dominated++;
                continue;
            }
            };
        }
    }

    while (!population_.empty())
    {
        results.push_back({});
        population_.sort([](Individual<T>* lhs, Individual<T>* rhs) {return lhs->dominated < rhs->dominated; });

        auto location = population_.begin();
        for (location; location != population_.end(); ++location)
        {
            if ((*location)->dominated != 0)
            {
                break;
            }
        }

        auto sequence = results.rbegin();
        (*sequence).splice((*sequence).end(), population_, population_.begin(), location);

        for (auto& individual : *results.rbegin())
        {
            for (auto& dominate : individual->dominates)
            {
                dominate->dominated--;
            }

            individual->dominates.clear();
        }
    }

    return results;
}

template<typename T>
void Population<T>::Evolve() {
    population_ = reproducer_->Reproduce(selector_->Select(sort()));

    for (auto& individual : individuals_)
    {
        fitness(*individual);
    }
}

template<typename T>
Series<T> Population<T>::Elite() 
{
    return *sort().begin();
}
#endif //!_MATH_OPTIMIZATION_UNSGA_POPULATION_