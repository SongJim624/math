#include "unsga.h"

int dominate(size_t length, const double* lhs, const double* rhs)
{
    std::array<size_t, 3> counts{ 0, 0, 0 };

    auto compare = [](double lhs, double rhs)
    {
        return std::abs(lhs - rhs) < 1e-7 ? 1 : (lhs > rhs ? 0 : 2);
    };

    for (size_t i = 0; i < length; ++i)
    {
        counts[compare(lhs[i], rhs[i])]++;
    }

    return (counts[1] == length) ? 0 : ((counts[0] == 0) ? 1 : ((counts[2] == 0) ? -1 : 0));
}

int dominate(size_t scale, size_t dimension, size_t constraint, const double* lhs, const double* rhs)
{
    int status = dominate(constraint, lhs + scale + dimension, rhs + scale + dimension);
    return status != 0 ? status : dominate(dimension, lhs + scale, rhs + scale);
}

bool dominate(size_t scale, size_t dimension, size_t constraint, double* individual, std::list<double*>& current, std::list<double*>& lower)
{
    auto member = current.begin();

    while (member != current.end())
    {
        switch (dominate(scale, dimension, constraint, *member, individual))
        {
        case 1:
        {
            return false;
        }
        case -1:
        {
            lower.push_back(*member);
            member = current.erase(member);
            continue;
        }
        case 0:
        {
            member++;
        }
        }
    }

    return true;
};

UNSGA::Population::Population(Optimizor::Configuration& configuration) :
    scale_(std::get<size_t>(configuration["scale"])),
    dimension_(std::get<size_t>(configuration["dimension"])),
    constraint_(std::get<size_t>(configuration["contraint"])),
    function_(*configuration.objective.get()),
    selector_(std::make_unique<Reference>(configuration)),
    reproducer_(std::make_unique<Reproducor>(configuration))
{
    size_t population = std::get<size_t>(configuration["population"]);
    std::vector<double> upper = std::get<std::vector<double>>(configuration["upper"]);
    std::vector<double> lower = std::get<std::vector<double>>(configuration["lower"]);
    std::vector<double> integer = std::get<std::vector<double>>(configuration["integer"]);

    std::random_device seed;
    std::mt19937_64 generator(seed());
    std::uniform_real_distribution<double> uniform(0, 1);

    auto generate = [&uniform, &generator, &upper, &lower, &integer](size_t length, double* decision)
    {
        auto temporary = std::unique_ptr<double, decltype(&math::free)>{ math::allocate(length), math::free };
        
        for (size_t i = 0; i < length; ++i)
        {
            decision[i] = uniform(generator);
        }

        math::sub(length, &upper[0], &lower[0], temporary.get());
        math::mul(length, temporary.get(), decision, decision);
        math::add(length, decision, &lower[0], decision);

        for (size_t i = 0; i < length; ++i)
        {
            decision[i] = integer[i] ? std::round(decision[i]) : decision[i];
        }
    };

    std::vector<std::vector<double>> initials;

    try
    {
        initials = std::get<std::vector<std::vector<double>>>(configuration["initials"]);
    }
    catch(...)
    {
    }

    while(initials.size() != population)
    {
        std::vector<double> decisions(scale_);
        generate(dimension_, &decisions[0]);
        initials.push_back(std::move(decisions));
    }

    for(const auto& decisions : initials)
    {
        double* individual = math::allocate(dimension_ + scale_ + constraint_);
        math::copy(scale_, &decisions[0], 1, individual, 1);
        individuals_.push_back(individual);
    }

    individuals_ = fitness(individuals_);
}

UNSGA::Population::~Population()
{
    for (auto& individual : individuals_)
    {
        std::free(individual);
        individual = nullptr;
    }
}

std::list<double*> UNSGA::Population::fitness(std::list<double*> population)
{
    for (auto individual : population)
    {
        function_(individual, individual + scale_, individual + scale_ + dimension_);
    }

    return population;
}

std::list<std::list<double*>> UNSGA::Population::sort(std::list<double*> individuals) const
{
    std::list<std::list<double*>> results{ {*individuals.begin()} };
    individuals.pop_front();
//mutually exclusive dominating and dominated in a layer when comparing all the individuals with a new one
// improvement of the non dominate sort
// if an individual is dominated by another which is in the upper layer,
// it must be dominated by the other individuals in the upper layer

    while (!individuals.empty())
    {        
        auto individual = *individuals.begin();
        individuals.pop_front();

        for (auto layer = results.begin(); layer != results.end(); ++layer)
        {
            std::list<double*> lower;
             bool status = dominate(scale_, dimension_, constraint_, individual, *layer, lower);

            if (status)
            {
                layer->push_back(individual);

                if (!lower.empty())
                {
                    results.insert(std::next(layer), lower);
                }

                break;
            }
            else
            {
                if (std::next(layer) == results.end())
                {
                    results.push_back({ individual });
                    break;
                }
                else
                {
                    continue;
                }
            }
        }
    }

    return results;
}

const math::Optimizor::Result& UNSGA::Population::evolve(size_t generation)
{
    for (size_t i = 0; i < generation; ++i)
    {
        individuals_ = fitness((*reproducer_)((*selector_)(sort(individuals_))));
    }

    return *this;
}