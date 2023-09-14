#include "unsga.h"

UNSGA::Population::Reproducor::Reproducor(math::Optimizor::Configuration& configuration) :
    scale_(std::get<size_t>(configuration["scale"])), dimension_(std::get<size_t>(configuration["dimension"])),
    function_(*configuration.objective),
    uppers_(math::allocate(scale_)), lowers_(math::allocate(scale_)), integers_(math::allocate(scale_)),
    cross_(std::get<double>(configuration["cross"])), mutation_(std::get<double>(configuration["mutation"])),
    threshold_(0.8), uniform_(std::uniform_real_distribution<double>(0, 1))
{
    std::random_device seed;
    generator_ = std::mt19937_64(seed());

    std::map<std::string, double*> constraints =
    {
        {"upper", uppers_},
        {"lower", lowers_},
        {"integer", integers_},
    };

    for (auto& [name, pointer] : constraints)
    {
        const auto& value = std::get<std::vector<double>>(configuration[name]);
        std::copy(value.begin(), value.end(), pointer);
    }
}

void UNSGA::Population::Reproducor::check(double * individual)
{
	for (size_t i = 0; i < scale_; ++i)
    {
		*individual = std::max(std::min(*individual, uppers_[i]), lowers_[i]);
        *individual = integers_[i] ? std::round(*individual) : *individual;

        individual++;
	}
}

void UNSGA::Population::Reproducor::cross(std::array<const double*, 2> parents, std::array<double*, 2> children)
{
    auto randoms = std::unique_ptr<double[], decltype(&math::free)>{ math::allocate(scale_), math::free };
    auto temporary = std::unique_ptr<double[], decltype(&math::free)>{ math::allocate(scale_), math::free };

    for(size_t i = 0; i < scale_; ++i)
    {
        randoms[i] = uniform_(generator_);
        randoms[i] = (randoms[i] < 0.5) ? 2.0 * randoms[i] : 0.5 / (1.0 - randoms[i]);
    }

    double probability = 1 / (cross_ + 1);
    math::powI(scale_, randoms.get(), 1, &probability, 0, randoms.get(), 1);

    auto& father = parents[0];
    auto& mother = parents[1];

    auto& son = children[0];
    auto& daughter = children[1];

    math::sub(scale_, father, mother, temporary.get());
    math::mul(scale_, randoms.get(), temporary.get(), temporary.get());

    math::add(scale_, father, mother, son);
    math::sub(scale_, son, temporary.get(), son);
    math::scal(scale_, 0.5, son, 1);

    math::add(scale_, father, mother, daughter);
    math::add(scale_, daughter, temporary.get(), daughter);
    math::scal(scale_, 0.5, daughter, 1);
}


void UNSGA::Population::Reproducor::mutate(double * individual)
{
    for (size_t i = 0; i < scale_; ++i)
    {
        double random = uniform_(generator_);
        double weight = ((random < 0.5) ? (uppers_[i] - individual[i]) : (individual[i] - lowers_[i])) / (uppers_[i] - lowers_[i]);

        double base = std::min(random, 1 - random);
        base = std::pow(2 * base + (1 - 2 * base) * std::pow(weight, mutation_ + 1), 1.0 / (mutation_ + 1.0));

        individual[i] += random <  0.5 ? base - 1 : 1 - base;
    }
}

std::list<double*> UNSGA::Population::Reproducor::operator ()(std::pair<std::list<double*>, std::list<double*>> population)
{
    std::list<double*> result, temporary;

    auto& elites = population.first;
    auto& ordinary = population.second;

    if (elites.size() % 2)
    {
        ordinary.push_front(*elites.rbegin());
        elites.pop_back();
    }

    auto father = elites.begin();
    auto mother = std::next(father);

    while (!ordinary.empty())
    {
        auto son = ordinary.rbegin();
        auto daughter = son++;

        cross({ *father, *mother }, { *son, *daughter });
        check(*son);
        check(*daughter);

        for (auto& individual : { *son, *daughter })
        {
            if (uniform_(generator_) > threshold_)
            {
                mutate(individual);
                check(individual);
            }
        }

        function_(*son, *son + scale_, *son + scale_ + dimension_);
        function_(*daughter, *daughter + scale_, *daughter + scale_ + dimension_);

        temporary.push_back(*son);
        temporary.push_back(*daughter);

        ordinary.pop_back();
        ordinary.pop_back();

        std::advance(father, 2);

        if (father == elites.end())
        {
            break;
        }

        std::advance(mother, 2);
    }

    result.splice(result.end(), elites);
    result.splice(result.end(), temporary);
    result.splice(result.end(), ordinary);

    return result;
}
