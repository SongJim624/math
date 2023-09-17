#include "unsga.h"

//  simulated binary crossover
void Reproducor::cross(const Individual parents[2], Individual children[2])
{
    auto randoms = std::unique_ptr<double[], decltype(&math::free)>{ math::allocate(scale_), math::free };
    auto temporary = std::unique_ptr<double[], decltype(&math::free)>{ math::allocate(scale_), math::free };
    auto father = parents[0], mother = parents[1], son = children[0], daughter = children[1];

    for(auto r = randoms.get(); r != randoms.get() + scale_; *r = uniform_(generator_), ++r)
    {
        *r = (*r < 0.5) ? 2.0 * *r : 0.5 / (1.0 - *r);
    }

    double probability = 1 / (cross_ + 1);
    math::powI(scale_, randoms.get(), 1, &probability, 0, randoms.get(), 1);

    math::sub(scale_, father, mother, temporary.get());
    math::mul(scale_, randoms.get(), temporary.get(), temporary.get());

    math::add(scale_, father, mother, son);
    math::sub(scale_, son, temporary.get(), son);
    math::scal(scale_, 0.5, son, 1);

    math::add(scale_, father, mother, daughter);
    math::add(scale_, daughter, temporary.get(), daughter);
    math::scal(scale_, 0.5, daughter, 1);
}

void Reproducor::mutate(Individual individual)
{
    for (size_t i = 0; i < scale_; ++i)
    {
        double random = uniform_(generator_);
        double weight = ((random < 0.5) ? (upper_[i] - individual[i]) : (individual[i] - lower_[i])) / (upper_[i] - lower_[i]);

        double base = std::min(random, 1 - random);
        base = std::pow(2 * base + (1 - 2 * base) * std::pow(weight, mutation_ + 1), 1.0 / (mutation_ + 1.0));

        individual[i] += random <  0.5 ? base - 1 : 1 - base;
    }
}

void Reproducor::check(Individual individual)
{
    for(auto value = individual, upper = upper_.get(), lower = lower_.get(), integer = integer_.get();
        value != individual + scale_; ++value, ++upper, ++lower, ++integer)
    {
		*value = std::max(std::min(*value, *upper), *lower);
        *value = *integer ? std::round(*value) : *value;
    }
}

Series Reproducor::reproduce(std::pair<Series, Series>&& population)
{
    auto& [elites, ordinaries] = population;
    Series offsprings = {};

//  by this way, elites will not be more than ordinaries
    if (elites.size() % 2)
    {
        ordinaries.push_front(std::move(*elites.rbegin()));
        elites.pop_back();
    }

    for(auto iter = elites.begin(); iter != elites.end() && !ordinaries.empty(); iter = std::next(iter, 2))
    {
        Individual parents[2] = { *iter, *std::next(iter) };
        Individual children[2] = { *ordinaries.rbegin(), *std::next(ordinaries.rbegin()) };

        cross(parents, children);

        for (auto& individual : children)
        {
            uniform_(generator_) > threshold_ ? mutate(individual) : void();
            check(individual);
            (*function_)(individual, individual + scale_, individual + scale_ + dimension_);
        }

        offsprings.insert(offsprings.end(), children, children + 2);
        ordinaries.pop_back();
        ordinaries.pop_back();
    }

    elites.splice(elites.end(), offsprings);
    elites.splice(elites.end(), ordinaries);
    return elites;
}

Reproducor::Reproducor(math::Optimizor::Configuration& configuration) :
    scale_(std::get<size_t>(configuration["scale"])), dimension_(std::get<size_t>(configuration["dimension"])),
    cross_(std::get<double>(configuration["cross"])), mutation_(std::get<double>(configuration["mutation"])), threshold_(0.8),
    upper_(math::allocate(scale_), math::free), lower_(math::allocate(scale_), math::free), integer_(math::allocate(scale_), math::free),
    function_(configuration.objective.get()), generator_(std::random_device()()), uniform_(0, 1)
{
    for(auto& [name, pointer] :
        std::map<std::string, double*>{ { "upper", upper_.get() }, { "lower", lower_.get() }, { "integer", integer_.get() } })
    {
        const auto& value = std::get<std::vector<double>>(configuration[name]);
        std::copy(value.begin(), value.end(), pointer);
    }
}