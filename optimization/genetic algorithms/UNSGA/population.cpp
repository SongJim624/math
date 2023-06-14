#include "unsga.h"
//=====================================================================
Individual::Individual(const std::vector<double>& decision) : decisions(decision)
{
	objectives.resize(Individual::dimensions, 0);
	voilations.resize(Individual::constraints, 0);
}

Population::Population(Configuration* configuration)
{
    objective_ = configuration->objective;
    constraint_ = configuration->constraint;
    selector_ = std::make_unique<Reference>(configuration);
    reproducer_ = std::make_unique<Reproducor>(configuration);

    for (size_t i = 0; i < configuration->population; ++i)
    {
        individuals_.push_back(std::make_unique<Individual>(configuration->initialization[i]));
        fitness(**individuals_.rbegin());
    }
}

Population::~Population()
{
    objective_ = nullptr;
    constraint_ = nullptr;
}

//=====================================================================

int dominate(size_t dimension, double* lhs, double* rhs)
{
	std::array<size_t, 3> counts{ 0, 0, 0 };

	auto compare = [](double lhs, double rhs, double tol = 1e-6) {
		return std::abs(lhs - rhs) < tol ? 1 : (lhs > rhs ? 0 : 2);
	};

	for (size_t i = 0; i < dimension; ++i) {
		counts[compare(lhs[i], rhs[i])]++;
	}

	return (counts[1] == dimension) ? 0 : ((counts[0] == 0) ? 1 : ((counts[2] == 0) ? -1 : 0));
}

//non dominated compare
int operator < (Individual& lhs, Individual& rhs)
{
	//need to further consider the multi non-linear constraints
	int status = Individual::constraints == 0 ? 0 : dominate(1, &lhs.voilations[0], &rhs.voilations[0]);

    return status != 0 ? status :
        dominate(Individual::dimensions, &lhs.objectives[0], &rhs.objectives[0]);
}

void Population::fitness(Individual& individual)
{
    (*objective_)(&individual.decisions[0], &individual.objectives[0]);
    (*constraint_)(&individual.decisions[0], &individual.objectives[0], Individual::constraints == 0 ? nullptr : &individual.voilations[0]);
}

Population::Layer Population::sort()
{
    Layer results{ {individuals_.begin()->get()} };
//mutually exclusive dominating and dominated in a layer when comparing all the individuals with a new one
// improvement of the non dominate sort
// if an individual is dominated by another which is in the upper layer, 
// it must be dominated by the other individuals in the upper layer
    auto rank = [](Individual* individual, std::list<Individual*>& layer, std::list<Individual*>& lower)
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

void Population::Evolve()
{    
    reproducer_->Reproduce(selector_->Select(sort()));

    for (auto& individual : individuals_)
    {
        fitness(*individual);
    }
}

std::list<Individual*> Population::Elite()
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
