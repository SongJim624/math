#include "unsga.hpp"

#ifndef _MATH_OPTIMIZATION_UNSGA_POPULATION_
#define _MATH_OPTIMIZATION_UNSGA_POPULATION_
template<typename T, class allocator>
UNSGA<T, allocator>::Population::Population(Optimizor<T, allocator>::Configuration* configuration)
{
    selector_ = std::make_unique<Reference>(configuration);
    reproducer_ = std::make_unique<Reproducor>(configuration);

    for (size_t i = 0; i < configuration->population; ++i)
    {
        individuals_.push_back(std::make_unique<Individual>(configuration->initialization[i]));

        auto& individual = **individual_.rbegin();
        *(configuration_->objective)(&individual.decisions[0], &individual.objectives[0]);
        *(configuration_->constraint)(&individual.decisions[0], &individual.objectives[0], &individual.voilations[0]);
    }
}

template<typename T, class allocator>
UNSGA<T, allocator>::Population::Layer UNSGA<T, allocator>::Population::sort()
{
    Layer results{ {individuals_.begin()->get()} };
//mutually exclusive dominating and dominated in a layer when comparing all the individuals with a new one
// improvement of the non dominate sort
// if an individual is dominated by another which is in the upper layer,
// it must be dominated by the other individuals in the upper layer
    auto dominate = [](size_t dimension, double* lhs, double* rhs)
    {
        std::array<size_t, 3> counts{ 0, 0, 0 };

        auto compare = [](double lhs, double rhs)
        {
            return std::abs(lhs - rhs) < 1e-7 ? 1 : (lhs > rhs ? 0 : 2);
        };

        for (size_t i = 0; i < dimension; ++i)
        {
            counts[compare(lhs[i], rhs[i])]++;
        }

        return (counts[1] == dimension) ? 0 : ((counts[0] == 0) ? 1 : ((counts[2] == 0) ? -1 : 0));
    };

    auto compare = [this](Individual& lhs, Individual& rhs)
    {
        //need to further consider the multi non-linear constraints
        int status = Individual::constraints == 0 ? 0 : dominate(1, &lhs.voilations[0], &rhs.voilations[0]);

        return status != 0 ? status :
            dominate(dimension_, &lhs.objectives[0], &rhs.objectives[0]);
    };

    auto rank = [](Individual* individual, std::list<Individual*>& layer, std::list<Individual*>& lower)
    {
        auto member = layer.begin();

        while (member != layer.end())
        {
            switch (compare(**member, *individual))
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
            std::list<Individual*> lower;
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

template<typename T, class allocator>
void UNSGA<T, allocator>::Population::Evolve()
{
    reproducer_->reproduce(selector_->select(sort()));

    for (auto& individual : individuals_)
    {
        *(configuration_->objective)(&individual.decisions[0], &individual.objectives[0]);
        *(configuration_->constraint)(&individual.decisions[0], &individual.objectives[0], &individual.voilations[0]);
    }
}
#endif //!_MATH_OPTIMIZATION_UNSGA_POPULATION_