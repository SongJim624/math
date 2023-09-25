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

Pointer create(size_t length)
{
    return Pointer(math::allocate<double>(length), math::free<double>);
}

void Population::evolve(size_t generation)
{
    for (size_t i = 0; i < generation; ++i)
    {
        std::cout << "executing the " << i << " th generation" << std::endl;
        individuals_ = reproducor_->reproduce(selector_->select(std::forward<Series>(individuals_)));
    }
}

void Population::write(const char * path)
{
	std::ofstream file(path);
	auto elites = selector_->sort(std::forward<Series>(individuals_));

	for (const auto& individual : *elites.begin())
//	for (const auto& individual : individuals_)
	{
		for (size_t i = 0; i < scale_ + dimension_ + constraint_; ++i)
		{
			file << individual[i] << "\t";
		}
		file << std::endl;
	}

	file.close();

/*
    while(!elites.empty())
    {
        individuals_.splice(individuals_.end(), *elites.begin());
        elites.pop_front();
    }
*/
}

Population::Population(math::Optimizor::Configuration& configuration) :
    scale_(std::get<size_t>(configuration["scale"])), 
    dimension_(std::get<size_t>(configuration["dimension"])), 
    constraint_(std::get<size_t>(configuration["constraint"])),
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

    while(initials.size() != population)
    {
        std::vector<double> decisions(scale_);

        for(auto& decision : decisions)
        {
            decision = uniform(generator);
        }

        generate(dimension_, &decisions[0], &upper[0], &lower[0], &integer[0]);
        initials.push_back(std::move(decisions));
    }

    for(const auto& decisions : initials)
    {
        population_.push_back(create(scale_ + dimension_ + constraint_));
        auto individual = population_.rbegin()->get();

        math::copy(scale_, &decisions[0], 1, individual, 1);
        (*configuration.objective)(individual, individual + scale_, individual + scale_ + dimension_);

        individuals_.push_back(individual);
    }
}