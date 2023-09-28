#include "unsga.h"

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
    selector_(std::make_unique<Reference>(configuration)), reproducor_(std::make_unique<Reproducor>(configuration))
{
    size_t population = std::get<size_t>(configuration["population"]);

    std::vector<double> upper = std::get<std::vector<double>>(configuration["upper"]);
    std::vector<double> lower = std::get<std::vector<double>>(configuration["lower"]);
    std::vector<double> integer = std::get<std::vector<double>>(configuration["integer"]);

    std::vector<std::vector<double>> initials;

    try
    {
        initials = std::get<std::vector<std::vector<double>>>(configuration["initials"]);
    }
    catch(...)
    {
    }

    std::random_device seed;
    std::mt19937_64 generator(seed());
    std::uniform_real_distribution<double> uniform(0, 1);

    individuals.resize(population);
    std::generate(individuals.begin(), individuals.end(), [this]() { return new Individual(scale, dimension, constraint); });

    auto initial = initials.begin();
    for(auto& individual : individuals)
    {
        if(initial == initials.end())
        {
            std::generate(individual->decisions, individual->decisions + scale, [&uniform, &generator]() { return uniform(generator); });
        }
        else
        {
            math::copy(scale, &(*initial)[0], 1, individual->decisions, 1);
            initial++;
        }

        (*configuration.objective)(individual->decisions, individual->objectives, individual->voilations);
    }
}

Population::~Population()
{
    for(auto& individual : individuals)
    {
        delete individual;
        individual = nullptr;
    }
}