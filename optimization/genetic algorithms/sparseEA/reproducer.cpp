#include "sparseea.h"

void evaluation(size_t scale, size_t dimension, math::Optimizor::Objective& function, Individual& individual)
{
    auto temporary = create<double>(scale);
    math::copy<double>(scale, individual.first, 1, temporary.get(), 1);

    for (size_t i = 0; i < scale; ++i)
    {
        individual.first[i] *= individual.second[i];
    }

    function(individual.first, individual.first + scale, individual.first + scale + dimension);
    math::copy<double>(scale, temporary.get(), 1, individual.first, 1);
}

// simulated binary cross over function for the decisions
void cross(double exponent, size_t scale, double *randoms, double* parents[2], double* children[2])
{
    auto temporary = create<double>(scale);
    auto father = parents[0], mother = parents[1], son = children[0], daughter = children[1];

    for (auto r = randoms; r != randoms + scale; ++r)
    {
        *r = (*r < 0.5) ? 2.0 * *r : 0.5 / (1.0 - *r);
    }

    double probability = 1 / (exponent + 1);
    math::powI(scale, randoms, 1, &probability, 0, randoms, 1);

    math::sub(scale, father, mother, temporary.get());
    math::mul(scale, randoms, temporary.get(), temporary.get());

    math::add(scale, father, mother, son);
    math::sub(scale, son, temporary.get(), son);
    math::scal(scale, 0.5, son, 1);

    math::add(scale, father, mother, daughter);
    math::add(scale, daughter, temporary.get(), daughter);
    math::scal(scale, 0.5, daughter, 1);
}

//  cross over function for the masks
void cross(double exponent, size_t scale, double* randoms, const std::map<size_t, std::list<size_t>>& importances, size_t* parents[2], size_t* children[2])
{




}

void mutate(double exponent, size_t scale, double * randoms, double* individual, double *upper, double * lower)
{
    for (size_t i = 0; i < scale; ++i)
    {
        double weight = ((randoms[i] < 0.5) ? (upper[i] - individual[i]) : (individual[i] - lower[i])) / (upper[i] - lower[i]);

        double base = std::min(randoms[i], 1 - randoms[i]);
        base = std::pow(2 * base + (1 - 2 * base) * std::pow(weight, exponent + 1), 1.0 / (exponent + 1.0));

        individual[i] += randoms[i] < 0.5 ? base - 1 : 1 - base;
    }
}

//  simulated binary crossover
void Reproducor::cross(const Individual parents[2], Individual children[2])
{
    auto randoms = create<double>(scale_);

    for (auto r = randoms.get(); r < randoms.get() + scale_; ++r)
    {
        *r = uniform_(generator_);
    }

    double* decisions[4] = { parents[0].first, parents[1].first, children[0].first, children[1].first };
    ::cross(cross_, scale_, randoms.get(), decisions, decisions + 2);

    size_t* masks[4] = { parents[0].second, parents[1].second, children[0].second, children[1].second };
    ::cross(cross_, scale_, randoms.get(), masks, masks + 2);
}

void Reproducor::mutate(Individual individual)
{
    auto randoms = create<double>(scale_);

    for (auto r = randoms.get(); r < randoms.get() + scale_; ++r)
    {
        *r = uniform_(generator_);
    }

    ::mutate(mutation_, scale_, randoms.get(), individual.first, upper_.get(), lower_.get());
}

void Reproducor::check(Individual individual)
{
    for(auto value = individual.first, upper = upper_.get(), lower = lower_.get(), integer = integer_.get();
        value != individual.first + scale_; ++value, ++upper, ++lower, ++integer)
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
            evaluation(scale_, dimension_, *function_, individual);
        }

        offsprings.insert(offsprings.end(), children, children + 2);
        ordinaries.pop_back();
        ordinaries.pop_back();
    }

    elites.splice(elites.end(), offsprings);
    elites.splice(elites.end(), ordinaries);
    return elites;
}

Reproducor::Reproducor(std::shared_ptr<std::map<size_t, std::list<size_t>>> importances, math::Optimizor::Configuration& configuration) :
    scale_(std::get<size_t>(configuration["scale"])), dimension_(std::get<size_t>(configuration["dimension"])),
    cross_(std::get<double>(configuration["cross"])), mutation_(std::get<double>(configuration["mutation"])), threshold_(0.8),
    upper_(create<double>(scale_)), lower_(create<double>(scale_)), integer_(create<double>(scale_)),
    function_(configuration.objective.get()), generator_(std::random_device()()), uniform_(0, 1)
{
    for(auto& [name, pointer] :
        std::map<std::string, double*>{ { "upper", upper_.get() }, { "lower", lower_.get() }, { "integer", integer_.get() } })
    {
        const auto& value = std::get<std::vector<double>>(configuration[name]);
        std::copy(value.begin(), value.end(), pointer);
    }
}