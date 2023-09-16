#include "unsga.h"

void generate(size_t scale, double *decisions, double *upper, double *lower, double *integer)
{
    auto temporary = std::unique_ptr<double, decltype(&math::free)>{ math::allocate(scale), math::free };

    math::sub(scale, upper, lower, temporary.get());
    math::mul(scale, temporary.get(), decisions, decisions);
    math::add(scale, decisions, &lower[0], decisions);

    for (size_t i = 0; i < scale; ++i)
    {
        decisions[i] = integer[i] ? std::round(decisions[i]) : decisions[i];
    }
}

void Population::evolve(size_t generation)
{
    for (size_t i = 0; i < generation; ++i)
    {
        individuals_ = reproducor_->reproduce(selector_->select(std::forward<Series>(individuals_)));
    }
}

void Population::write(const char * path)
{
	std::ofstream file(path);
	auto&& elites = std::move(selector_->sort(std::forward<Series>(individuals_)));

	for (const auto& individual : *elites.begin())
	{
		for (size_t i = 0; i < scale_ + dimension_ + constraint_; ++i)
		{
			file << individual[i] << "\t";
		}
		file << std::endl;
	}

	file.close();

    while(!elites.empty())
    {
        individuals_.splice(individuals_.end(), *elites.begin());
        elites.pop_front();
    }
}

Population::Population(math::Optimizor::Configuration& configuration) :
    selector_(std::make_unique<Reference>(configuration)), reproducor_(std::make_unique<Reproducor>(configuration))
{
    size_t scale = std::get<size_t>(configuration["scale"]);
    size_t dimension = std::get<size_t>(configuration["dimension"]);
    size_t constraint = std::get<size_t>(configuration["constraint"]);
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

    while(initials.size() != population)
    {
        std::vector<double> decisions(scale);

        for(auto& decision : decisions)
        {
            decision = uniform(generator);
        }

        generate(dimension, &decisions[0], &upper[0], &lower[0], &integer[0]);
        initials.push_back(std::move(decisions));
    }

    for(const auto& decisions : initials)
    {
        population_.push_back(std::unique_ptr<double[], decltype(&math::free)>(math::allocate(scale + dimension + constraint), math::free));
        auto individual = population_.rbegin()->get();

        math::copy(scale, &decisions[0], 1, individual, 1);
        (*configuration.objective)(individual, individual + scale, individual + scale + dimension);

        individuals_.push_back(individual);
    }
}