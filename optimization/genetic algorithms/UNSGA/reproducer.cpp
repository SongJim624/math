#include "unsga.h"

//  simulated binary crossover
void Reproducor::cross(const Individual& father, const Individual& mother, Individual& son, Individual& daughter)
{
    auto randoms = math::allocate<double>(scale_);
    auto temporary = math::allocate<double>(scale_);
//    auto father = parents[0], mother = parents[1], son = children[0], daughter = children[1];

    for(auto r = randoms.get(); r != randoms.get() + scale_; *r = uniform_(generator_), ++r)
    {
        *r = (*r < 0.5) ? 2.0 * *r : 0.5 / (1.0 - *r);
    }

    math::pow(scale_, randoms.get(), 1, 1 / (cross_ + 1), randoms.get(), 1);

    math::sub(scale_, father.decisions, 1, mother.decisions, 1, temporary.get(), 1);
    math::mul(scale_, randoms.get(), 1, temporary.get(), 1, temporary.get(), 1);

    math::add(scale_, father.decisions, 1, mother.decisions, 1, son.decisions, 1);
    math::sub(scale_, son.decisions, 1, temporary.get(), 1, son.decisions, 1);
    math::scal(scale_, 0.5, son.decisions, 1);

    math::add(scale_, father.decisions, 1, mother.decisions, 1, daughter.decisions, 1);
    math::add(scale_, daughter.decisions, 1, temporary.get(), 1, daughter.decisions, 1);
    math::scal(scale_, 0.5, daughter.decisions, 1);

    check(son);
    check(daughter);
}

void Reproducor::mutate(Individual& individual)
{
    for (size_t i = 0; i < scale_; ++i)
    {
        double random = uniform_(generator_);
        double weight = ((random < 0.5) ? (upper_[i] - individual.decisions[i]) : (individual.decisions[i] - lower_[i])) / (upper_[i] - lower_[i]);

        double base = std::min(random, 1 - random);
        base = std::pow(2 * base + (1 - 2 * base) * std::pow(weight, mutation_ + 1), 1.0 / (mutation_ + 1.0));

        individual.decisions[i] += random <  0.5 ? base - 1 : 1 - base;
    }

    check(individual);
}

void Reproducor::check(Individual& individual)
{
    for(auto value = individual.decisions, upper = upper_.get(), lower = lower_.get(), integer = integer_.get();
        value != individual.decisions + scale_; ++value, ++upper, ++lower, ++integer)
    {
		*value = std::max(std::min(*value, *upper), *lower);
        *value = *integer ? std::round(*value) : *value;
    }
}

std::list<Individual*> Reproducor::reproduce(std::pair<std::list<Individual*>, std::list<Individual*>>&& population)
{
    auto& [elites, ordinaries] = population;

    std::list<Individual*> offsprings = {};

//  by this way, elites will not be more than ordinaries
    if (elites.size() % 2)
    {
        ordinaries.push_front(*elites.rbegin());
        elites.pop_back();
    }

    ordinaries.reverse();
    for(auto iter = elites.begin(); iter != elites.end() && !ordinaries.empty(); iter = std::next(iter, 2))
    {
        cross(**iter, **std::next(iter), **ordinaries.begin(), **std::next(ordinaries.begin()));

        for (size_t i = 0; i < 2;  ++i)
        {
            auto& child = **std::next(ordinaries.begin(), i);
            uniform_(generator_) > threshold_ ? mutate(child) : void();
            (*function_)(child.decisions, child.objectives, child.voilations);
        }

        offsprings.splice(offsprings.end(), ordinaries, ordinaries.begin(), std::next(ordinaries.begin(), 2));
    }

    elites.splice(elites.end(), offsprings);
    elites.splice(elites.end(), ordinaries);
    return elites;
}

Reproducor::Reproducor(math::Optimizor::Configuration& configuration) :
    scale_(std::get<size_t>(configuration["scale"])), dimension_(std::get<size_t>(configuration["dimension"])),
    cross_(std::get<double>(configuration["cross"])), mutation_(std::get<double>(configuration["mutation"])), threshold_(0.8),
    upper_(math::allocate<double>(scale_)), lower_(math::allocate<double>(scale_)), integer_(math::allocate<double>(scale_)),
    function_(configuration.objective.get()), generator_(std::random_device()()), uniform_(0, 1)
{
    for(auto& [name, pointer] :
        std::map<std::string, double*>{ { "upper", upper_.get() }, { "lower", lower_.get() }, { "integer", integer_.get() } })
    {
        const auto& value = std::get<std::vector<double>>(configuration[name]);
        std::copy(value.begin(), value.end(), pointer);
    }
}