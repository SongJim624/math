#include "unsga.hpp"

#ifndef _MATH_OPTIMIZATION_UNSGA_REPRODUCER_
#define _MATH_OPTIMIZATION_UNSGA_REPRODUCER_
template<typename T, class allocator>
void UNSGA<T, allocator>::Population::Reproducor::check(Individual& individual)
{
	for (size_t i = 0; i < scale_; ++i) {
		individual.decisions[i] = std::max(std::min(individual.decisions[i], uppers_[i]), lowers_[i]);

		if (integers_[i]) {
			individual.decisions[i] = round(individual.decisions[i]);
		}
	}
}

template<typename T, class allocator>
UNSGA<T, allocator>::Population::Reproducor::Reproducor(Optimizor<T, allocator>::Configuration* configuration)
{
    scale_ = configuration->scales;

    uppers_ = std::get<std::vector<T, allocator>>(*configuration["uppers"]);
    lowers_ = std::get<std::vector<T, allocator>>(*configuration["lowers"]);
    integers_ = std::get<std::vector<T, allocator>>(*configuration["integers"]);

    cross_ = std::get<T>(*configuration["cross"]);
    mutation_ = std::get<T>(*configuration["mutation"]);
    threshold_ = std::get<T>(*configuration["threshold"]);
}

template<typename T, class allocator>
void UNSGA<T, allocator>::Population::Reproducor::cross(std::array<const Individual*, 2> parents, std::array<Individual*, 2> children)
{
    std::vector<T, allocator> randoms(scale_);

    for(auto& random : randoms)
    {
        random = uniform_(generator_);
        randoms = (randoms < 0.5) ? 2.0 * randoms : 0.5 / (1.0 - randoms);
    }

    randoms = randoms ^ (1 / (cross_ + 1));

    auto& father = parents[0]->decisions;
    auto& mother = parents[1]->decisions;

    auto& son = children[0]->decisions;
    auto& daughter = children[1]->decisions;

    son = 0.5 * (father + mother - randoms * (father - mother));
    daughter = 0.5 * (father + mother + randoms * (father - mother));
}

template<typename T, class allocator>
void UNSGA<T, allocator>::Population::Reproducor::mutate(Individual& individual)
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

template<typename T, class allocator>
UNSGA<T, allocator>::Population::Series UNSGA<T, allocator>::Population::Reproducor::reproduce(std::pair<Series, Series> population)
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
