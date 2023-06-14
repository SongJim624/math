#include "unsga.h"
void Reproducor::check(Individual& individual)
{
	for (size_t i = 0; i < scale_; ++i)
    {
		individual.decisions[i] = std::max(std::min(individual.decisions[i], uppers_[i]), lowers_[i]);

		if (integers_[i])
        {
			individual.decisions[i] = round(individual.decisions[i]);
		}
	}
}

Reproducor::Reproducor(Configuration* configuration)
{
    scale_ = configuration->scales;

    uppers_ = configuration->uppers;
    lowers_ = configuration->lowers;
    integers_ = configuration->integers;

    cross_ = configuration->cross;
    mutation_ = configuration->mutation;
    threshold_ = 0.8;
}

void Reproducor::Cross(std::array<const Individual*, 2> parents, std::array<Individual*, 2> children)
{
    std::vector<double> randoms(scale_);
    for (size_t i = 0; i < scale_; ++i)
    {
        randoms[i] = uniform_(generator_);
        randoms[i] = (randoms[i] < 0.5) ? 2.0 * randoms[i] : 0.5 / (1.0 - randoms[i]);
    }

    randoms = randoms ^ (1 / (cross_ + 1));

    auto& father = parents[0]->decisions;
    auto& mother = parents[1]->decisions;

    auto& son = children[0]->decisions;
    auto& daughter = children[1]->decisions;

    son = 0.5 * (father + mother - randoms * (father - mother));
    daughter = 0.5 * (father + mother + randoms * (father - mother));
}

void Reproducor::Mutate(Individual* individual)
{
    auto update = [this](double random, double lower, double upper, double& decision)
    {
        double weight = (upper - decision) / (upper - lower);

        decision = (random < 0.5) ?
            pow(2 * random + (1 - 2 * random) * pow(weight, mutation_ + 1), 1 / (1 + mutation_)) - 1:
            1 - pow(2 * (1 - random) + (2 * random - 1) * pow(weight, mutation_ + 1), 1 / (1 + mutation_));
    };

    for (size_t i = 0; i < scale_; ++i)
    {
        update(uniform_(generator_), lowers_[i], uppers_[i], individual->decisions[i]);
    }
}

std::list<Individual*> Reproducor::Reproduce(std::pair<std::list<Individual*>, std::list<Individual*>> population)
{
    std::list<Individual*> result, temporary;

    auto& elites = population.first;
    auto& ordinary = population.second;

    if (elites.size() % 2)
    {
        ordinary.push_front(*elites.rbegin());
        elites.pop_back();
    }

    auto father = elites.begin();
    auto mother = std::next(father);

    while (true)
    {
        if (ordinary.empty())
        {
            break;
        }

        auto son = ordinary.rbegin();
        auto daughter = std::next(son);

        Cross({ *father, *mother }, { *son, *daughter });

        if (uniform_(generator_) > threshold_)
        {
            Mutate(*son);
        }

        if (uniform_(generator_) > threshold_)
        {
            Mutate(*daughter);
        }

        check(**son);
        check(**daughter);

        temporary.push_back(*son);
        temporary.push_back(*daughter);

        ordinary.pop_back();
        ordinary.pop_back();

        std::advance(father, 2);
        if (father == elites.end()) {
            break;
        }

        std::advance(mother, 2);
    }

    result.splice(result.end(), elites);
    result.splice(result.end(), temporary);
    result.splice(result.end(), ordinary);
    return result;
}