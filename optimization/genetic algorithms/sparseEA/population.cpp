#include "sparseEA.h"

void generate(size_t scale, double *decisions, double *upper, double *lower, double *integer)
{
    auto temporary = create(scale);

    math::sub(scale, upper, lower, temporary.get());
    math::mul(scale, temporary.get(), decisions, decisions);
    math::add(scale, decisions, &lower[0], decisions);

    for (size_t i = 0; i < scale; ++i)
    {
        decisions[i] = integer[i] ? std::round(decisions[i]) : decisions[i];
    }
}

Evolutionary::Selector<Individual>& Population::selector()
{
    return *selector_;
}

Evolutionary::Reproducor<Individual>& Population::reproducor()
{
    return *reproducor_;
}

Population::Population(math::Optimizor::Configuration& configuration) :
    scale(std::get<size_t>(configuration["scale"])),
    dimension(std::get<size_t>(configuration["dimension"])),
    constraint(std::get<size_t>(configuration["constraint"])),
    importances(new size_t[scale]),
    selector_(std::make_unique<Reference>(configuration)), 
    reproducor_(nullptr)
{
    size_t population = std::get<size_t>(configuration["population"]);

    std::vector<double> upper = std::get<std::vector<double>>(configuration["upper"]);
    std::vector<double> lower = std::get<std::vector<double>>(configuration["lower"]);
    std::vector<double> integer = std::get<std::vector<double>>(configuration["integer"]);

    std::vector<std::vector<double>> initials;

    try
    {
        initials = std::get<std::vector<std::vector<double>>>(configuration["initial"]);
    }
    catch(...)
    {
    }

    std::random_device seed;
    std::mt19937_64 generator(seed());
    std::uniform_real_distribution<double> uniform(0, 1);

    individuals.resize(scale);
    std::generate(individuals.begin(), individuals.end(), [this]() { return new Individual(scale, dimension, constraint); });

//  make sure that the population size is larger than the scale
    auto temporary = create(scale);
    for(auto individual = individuals.begin(); individual != std::next(individuals.begin(), scale); ++individual)
    {
        auto masks = (*individual)->masks;
        auto decisions = (*individual)->decisions;
        auto objectives = (*individual)->objectives;
        auto voilations = (*individual)->voilations;

        masks[std::distance(individuals.begin(), individual)] = 1;
        std::generate(decisions, decisions + scale, [&uniform, &generator]() { return uniform(generator); });
        generate(scale, decisions, &upper[0], &lower[0], &integer[0]);

        math::mul(scale, decisions, masks, temporary.get());
        (*configuration.objective)(temporary.get(), objectives, voilations);
    }

    auto layers = selector_->sort(individuals);
    for(auto layer = layers.begin(); layer != layers.end(); ++layer)
    {
        for(const auto& individual : *layer)
        {
            for(size_t i = 0; i < scale; ++i)
            {
                if(individual->masks[i])
                {
                    importances[i] = std::distance(layers.begin(), layer);
                }
            }
        }
    }
    reproducor_ = std::make_unique<Reproducor>(configuration, importances);

    individuals.resize(population);
    std::generate(std::next(individuals.begin(), scale), individuals.end(), [this]() { return new Individual(scale, dimension, constraint); });
    auto initial = initials.begin();
    for(auto individual = std::next(individuals.begin(), scale); individual != individuals.end(); ++individual)
    {
        auto masks = (*individual)->masks;
        auto decisions = (*individual)->decisions;
        auto objectives = (*individual)->objectives;
        auto voilations = (*individual)->voilations;

    //  generate the decision variables
        if(initial == initials.end())
        {
            std::generate(decisions, decisions + scale, [&uniform, &generator]() { return uniform(generator); });
            generate(scale, decisions, &upper[0], &lower[0], &integer[0]);
        }
        else
        {
            math::copy(scale, &(*initial)[0], 1, decisions, 1);
            initial++;
        }

    //  generate the masks
        size_t amount = std::uniform_int_distribution<>(0, scale - 1)(generator);
        for(size_t i = 0; i < amount; ++i)
        {
            size_t first = std::uniform_int_distribution<>(0, scale - 1)(generator);
            size_t second = std::uniform_int_distribution<>(0, scale - 1)(generator);
            
            masks[importances[first] < importances[second] ? first : second] = 1;
        }

        math::mul(scale, decisions, masks, temporary.get());
        (*configuration.objective)(temporary.get(), objectives, voilations);
    }
}

Population::~Population()
{
    delete[] importances;
    importances = nullptr;

    for(auto& individual : individuals)
    {
        delete individual;
        individual = nullptr;
    }
}