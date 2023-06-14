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

	std::list<std::unique_ptr<Individual<T>>> individuals_;

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
Population<T>::Population(Configuration<T>* configuration)
{
    objective_ = configuration->objective;
    constraint_ = configuration->constraint;
    selector_ = std::make_unique<Reference<T>>(configuration);
    reproducer_ = std::make_unique<Reproducor<T>>(configuration);

    for (size_t i = 0; i < configuration->population; ++i)
    {
        individuals_.push_back(std::make_unique<Individual<T>>(configuration->initialization[i]));
        fitness(**individuals_.rbegin());
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
    Layer<T> results{ {individuals_.begin()->get()} };
//mutually exclusive dominating and dominated in a layer when comparing all the individuals with a new one
// improvement of the non dominate sort
// if an individual is dominated by another which is in the upper layer, 
// it must be dominated by the other individuals in the upper layer
    auto rank = [](Individual<T>* individual, std::list<Individual<T>*>& layer, std::list<Individual<T>*>& lower)
    {
        auto member = layer.begin();

        while (member != layer.end())
        {
            switch (**member < *individual)
            {
            case 1:
            {
                return false;
            }
            case -1:
            {
                lower.push_back(*member);
                member = layer.erase(member);

                if (member == layer.end())
                {
                    return true;
                }
            }
            case 0:
            {
                member++;
            }
            }
        }

        return true;
    };

    for (auto individual = std::next(individuals_.begin()); individual != individuals_.end(); ++individual)
    {
        for (auto layer = results.begin(); layer != results.end(); ++layer)
        {
            std::list<Individual<T>*> lower;
            bool status = rank(individual->get(), *layer, lower);

            if (!status && std::next(layer) != results.end())
            {
                results.insert(results.end(), { individual->get() });
                break;
            }

            if(status)
            {
                layer->push_back(individual->get());

                if (!lower.empty())
                {
                    results.insert(std::next(layer), lower);
                }

                break;
            }
        }
    }
    return results;
}

template<typename T>
void Population<T>::Evolve()
{    
    reproducer_->Reproduce(selector_->Select(sort()));

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

/*
template<typename T>
Layer<T> Population<T>::sort() {
    Layer<T> results;

    for (auto individual = individuals_.begin(); individual != individuals_.end(); ++individual)
    {
        for (auto later = std::next(individual); later != individuals_.end(); ++later)
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
*/
#endif //!_MATH_OPTIMIZATION_UNSGA_POPULATION_