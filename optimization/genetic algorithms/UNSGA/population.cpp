#include "unsga.h"

UNSGA::Population::Population(Optimizor::Configuration& configuration) :
    function_(*configuration.objective.get()),
    selector_(std::make_unique<Reference>(configuration)),
    reproducer_(std::make_unique<Reproducor>(configuration))
{
    size_t scale = std::get<size_t>(configuration["scale"]);
    size_t constraint = std::get<size_t>(configuration["constraint"]);
    size_t dimension = std::get<size_t>(configuration["dimension"]);
    size_t population = std::get<size_t>(configuration["population"]);

    std::list<std::vector<double>> initials;

    try
    {
        initials = std::get<std::list<std::vector<double>>>(configuration["initials"]);
    }
    catch(std::bad_alloc exception)
    {
    }

    while(initials.size() < population)
    {

    };



    for(const auto& decisions : initials)
    {
        auto individual = new Individual(dimension, scale, constraint);
        math::copy(scale, &decisions[0], 1, individual->decisions, 1);

        function_(individual->decisions, individual->objectives, individual->voilations);
        individuals_.push_back(individual);
    }
}

std::list<UNSGA::Population::Series> UNSGA::Population::sort(Series individuals)
{
    std::list<Series> results{ {individuals_.begin()->get()} };
//mutually exclusive dominating and dominated in a layer when comparing all the individuals with a new one
// improvement of the non dominate sort
// if an individual is dominated by another which is in the upper layer,
// it must be dominated by the other individuals in the upper layer
    auto dominate = [](size_t dimension, T* lhs, T* rhs)
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

void UNSGA::Population::Evolve()
{
    individuals_ = *reproduccer_(*selector_(sort()));

    for (auto& individual : individuals_)
    {
        *(configuration_->objective)(individual.decisions, individual.objectives, voilation);
    }
}