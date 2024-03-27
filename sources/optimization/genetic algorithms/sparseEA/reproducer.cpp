#include "sparseEA.h"

void check(size_t scale, Individual& individual, const double* uppers, const double* lowers, const double* integers)
{
    for(size_t i = 0;i < scale; ++i)
    {
        individual.decisions[i] = integers[i] ? std::round(individual.decisions[i]) : individual.decisions[i];
        individual.decisions[i] = std::max(std::min(individual.decisions[i], uppers[i]), lowers[i]);
    }
}

//  simulate binary crossover for the decision variables
void cross(size_t scale, double exponent, std::mt19937_64& generator, std::array<const Individual*, 2> parents, std::array<Individual*, 2> children, const double * uppers, const double* lowers, const double* integers)
{
    const auto &father = *parents[0], &mother = *parents[1];
    auto &son = *children[0], &daughter = * children[1];

    auto randoms = create(scale);
    std::generate(randoms.get(), randoms.get() + scale, [&generator](){ return std::uniform_real_distribution<>(0.0, 1.0)(generator); });

    auto temporary = create(scale);

    for(auto r = randoms.get(); r < randoms.get() + scale; ++r)
    {
        *r = (*r < 0.5) ? 2.0 * *r : 0.5 / (1.0 - *r);
    }

    double probability = 1 / (exponent + 1);
    math::powI(scale, randoms.get(), 1, &probability, 0, randoms.get(), 1);

    math::sub(scale, father.decisions, mother.decisions, temporary.get());
    math::mul(scale, randoms.get(), temporary.get(), temporary.get());

    math::add(scale, father.decisions, mother.decisions, son.decisions);
    math::sub(scale, son.decisions, temporary.get(), son.decisions);
    math::scal(scale, 0.5, son.decisions, 1);

    math::add(scale, father.decisions, mother.decisions, daughter.decisions);
    math::add(scale, daughter.decisions, temporary.get(), daughter.decisions);
    math::scal(scale, 0.5, daughter.decisions, 1);

    check(scale, son, uppers, lowers, integers);
    check(scale, daughter, uppers, lowers, integers);
}
//  polynomial mutation for decision variables
void mutate(size_t scale, double exponent, std::mt19937_64& generator, Individual& individual, const double* uppers, const double* lowers, const double* integers)
{
    auto randoms = create(scale);
    std::generate(randoms.get(), randoms.get() + scale, [&generator]() { return std::uniform_real_distribution<>(0.0, 1.0)(generator); });

    for (size_t i = 0; i < scale; ++i)
    {
        double weight = ((randoms[i] < 0.5) ? (uppers[i] - individual.decisions[i]) : (individual.decisions[i] - lowers[i])) / (uppers[i] - lowers[i]);

        double base = std::min(randoms[i], 1 - randoms[i]);
        base = std::pow(2 * base + (1 - 2 * base) * std::pow(weight, exponent + 1), 1.0 / (exponent + 1.0));

        individual.decisions[i] += randoms[i] <  0.5 ? base - 1 : 1 - base;
    }

    check(scale, individual, uppers, lowers, integers);
}

void flip(size_t scale, double random, std::mt19937_64& generator, double * individual, const double* masks, const size_t*importances)
{
    std::vector<size_t> selections(0);

    size_t count = 0;
    for (size_t i = 0; i < scale; ++i)
    {
        count += (masks[i] == 0);
    }

    while(selections.size() != scale - count)
    {
        size_t selection = std::uniform_int_distribution<>(0, scale - 1)(generator);
        masks[selection] ? selections.push_back(selection) : void();
    }

    switch (selections.size())
    {
    case 0:
    {
        return;
    }
    case 1:
    {
        individual[selections[0]] = 1 - individual[selections[0]];
        return;
    }
    default:
    {
        size_t selection = (random > 0.5) - (importances[selections[0]] < importances[selections[1]]);
        individual[selection] = 1 - individual[selection];
        return;
    }
    }
}

void cross(size_t scale, std::mt19937_64& generator, std::array<const Individual*, 2> parents, std::array<Individual*, 2> children, const size_t* importances)
{
    double one = 1;
    auto masks = create(scale);

    for(auto& child : children)
    {
        math::copy(scale, parents[0]->masks, 1, child->masks, 1);

        double random = std::uniform_real_distribution<>(0.0, 1.0)(generator);
        math::subI(scale, &one, 0, parents[random < 0.5]->masks, 1, masks.get(), 1);
        math::addI(scale, parents[random > 0.5]->masks, 1, masks.get(), 1, masks.get(), 1);

        flip(scale, random < 0.5, generator, child->masks, masks.get(), importances);
    }
}

void mutate(size_t scale, std::mt19937_64& generator, Individual& individual, const size_t* importances)
{
    auto masks = create(scale);
    double one = 1.0, random = std::uniform_real_distribution<>(0.0, 1.0)(generator);
    random < 0.5 ? math::copy<double>(scale, individual.masks, 1, masks.get(), 1) : math::subI(scale, &one, 0, individual.masks, 1, masks.get(), 1);
    flip(scale, random < 0.5, generator, individual.masks, masks.get(), importances);
}

//  simulated binary crossover
void Reproducor::cross(const Individual& father, const Individual& mother, Individual& son, Individual& daughter)
{
//  cross the masks
    ::cross(scale_, generator_, { &father, &mother },  { &son, &daughter }, importances_);
//  cross the decisions
    ::cross(scale_, cross_, generator_, { &father, &mother }, { &son, &daughter }, upper_.get(), lower_.get(), integer_.get());
}

void Reproducor::mutate(Individual& individual)
{
//  mutate the masks
    ::mutate(scale_, generator_, individual, importances_);
//  mutate the decision variables
    ::mutate(scale_, mutation_, generator_, individual, upper_.get(), lower_.get(), integer_.get());
}

void Reproducor::check(Individual& individual)
{
    assert((void("not implemented!"), true));
}

std::list<Individual*> Reproducor::reproduce(std::pair<std::list<Individual*>, std::list<Individual*>>&& population)
{
    auto temporary = create(scale_);
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
            std::uniform_real_distribution<>(0.0, 1.0)(generator_) > threshold_ ? mutate(child) : void();

            math::mul(scale_, child.decisions, child.masks, temporary.get());
            (*function_)(temporary.get(), child.objectives, child.voilations);
        }

        offsprings.splice(offsprings.end(), ordinaries, ordinaries.begin(), std::next(ordinaries.begin(), 2));
    }

    elites.splice(elites.end(), offsprings);
    elites.splice(elites.end(), ordinaries);
    return elites;
}

Reproducor::Reproducor(math::Optimizor::Configuration& configuration, size_t *importances) :
    scale_(std::get<size_t>(configuration["scale"])), dimension_(std::get<size_t>(configuration["dimension"])),
    cross_(std::get<double>(configuration["cross"])), mutation_(std::get<double>(configuration["mutation"])), threshold_(0.8),
    upper_(create(scale_)), lower_(create(scale_)), integer_(create(scale_)),
    function_(configuration.objective.get()), generator_(std::random_device()()), importances_(importances)
{
    for(auto& [name, pointer] :
        std::map<std::string, double*>{ { "upper", upper_.get() }, { "lower", lower_.get() }, { "integer", integer_.get() } })
    {
        const auto& value = std::get<std::vector<double>>(configuration[name]);
        std::copy(value.begin(), value.end(), pointer);
    }
}

Reproducor::~Reproducor()
{
    importances_ = nullptr;
    function_ = nullptr;
}