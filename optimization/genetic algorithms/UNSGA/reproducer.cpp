#include "unsga.h"

UNSGA::Population::Reproducor::Reproducor(const math::Optimizor::Configuration& configuration) :
    dimension(std::get<size_t>(configuration["dimension"])),
    scale(std::get<size_t>(configuration["sacle"])),
    cross_(std::get<double>(configuration["cross"])),
    mutation_(std::get<double>(configuration["mutation"])),
    threshold_(std::get<double>(configuration["threshold"]))
{

}


void UNSGA::Population::Reproducor::check(Individual& individual)
{
	for (size_t i = 0; i < scale; ++i)
    {
        auto& value = individual.decisions[i];
		value = std::max(std::min(individual.decisions[i], uppers_[i]), lowers_[i]);
        value = integers_[i] ? std::round(value) : value;
	}
}

UNSGA::Population::Reproducor::Reproducor(const Optimizor::Configuration& configuration)
{
    scale_ = configuration->scales;

    uppers_ = std::get<std::vector<double>>(*configuration["uppers"]);
    lowers_ = std::get<std::vector<T, allocator>>(*configuration["lowers"]);
    integers_ = std::get<std::vector<T, allocator>>(*configuration["integers"]);

    cross_ = std::get<double>(configuration["cross"]);
    mutation_ = std::get<double>(configuration["mutation"]);
    threshold_ = std::get<double>(configuration["threshold"]);
}


void UNSGA::Population::Reproducor::cross(std::array<const Individual*, 2> parents, std::array<Individual*, 2> children)
{
    std::vector<T, allocator> randoms(scale_);

    for(auto& random : randoms)
    {
        random = uniform_(generator_);
        random = (random < 0.5) ? 2.0 * random : 0.5 / (1.0 - random);
    }

    math::pow(scale, randoms, 1 / (cross_ + 1), randoms);

    auto& father = parents[0]->decisions;
    auto& mother = parents[1]->decisions;

    auto& son = children[0]->decisions;
    auto& daughter = children[1]->decisions;

    son = 0.5 * (father + mother - randoms * (father - mother));
    daughter = 0.5 * (father + mother + randoms * (father - mother));
}


void UNSGA::Population::Reproducor::mutate(Individual& individual)
{
    for (size_t i = 0; i < scale_; ++i)
    {
        T& decision = individual.decisions[i];

        T random = uniform_(generator_);
        T weight = ((ramdom < 0.5) ? (uppers_[i] - decision) : (decision - lowers_[i])) / (uppers_[i] - lowers[i]);
        T base = std::min(random, 1 - random);
        base = std::pow(2 * base + (1 - 2 * base) * std::pow(weight, mutation_ + 1), 1.0 / (mutation_ + 1.0));

        decision = random <  0.5 ? base - 1 :  1 - base;
    }
}

UNSGA::Population::Series UNSGA::Population::Reproducor::reproduce(std::pair<Series, Series> population)
{
    Series result, temporary;

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

        cross({ *father, *mother }, { *son, *daughter });

        for(auto& individual : {father, mother, son, daughter})
        {
            if(uniform_(generator_))
            {
                mutate(*individual);
                check(**individual);
            }
        }

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

#endif //!_MATH_OPTIMIZATION_UNSGA_REPRODUCER_
