#include "sparseea.h"

void generate(size_t scale, double *decisions, double *upper, double *lower, double *integer)
{
    auto temporary = create<double>(scale);

    math::sub(scale, upper, lower, temporary.get());
    math::mul(scale, temporary.get(), decisions, decisions);
    math::add(scale, decisions, &lower[0], decisions);

    for (size_t i = 0; i < scale; ++i)
    {
        decisions[i] = integer[i] ? std::round(decisions[i]) : decisions[i];
    }
}

/******************************************************************************
* compute the importance (fitness of the decision variables in the article)
*********************************************************************************/

void Population::importances(double* decisions, math::Optimizor::Objective& function)
{
    Series individuals(scale_);
    std::list<Pointer<double>> pointers;

    for (auto individual = individuals.begin(); individual != individuals.end(); ++individual)
    {
        pointers.push_back(create<double>(scale_ + dimension_ + constraint_));

        auto pointer = pointers.rbegin()->get();
        size_t pos = std::distance(individuals.begin(), individual);
        pointer[pos] = *decisions++;

        function(pointer, pointer + scale_, pointer + scale_ + dimension_);

        individual->first = pointer;
        individual->second = nullptr;
    }

//    auto copy = individuals;
    auto layers = selector_->sort(std::forward<Series>(individuals));

    for (auto layer = layers.begin(); layer != layers.end(); ++layer)
    {
        size_t index = std::distance(layers.begin(), layer);
        (*importances_).insert({ index, {} });

        for (const auto& individual : *layer)
        {
            auto pos = std::find(individuals.begin(), individuals.end(), individual);
            (*importances_)[index].push_back(std::distance(layers.begin(), layer));
        }
    }
}

/******************************************************************************
* constructors of the population
*********************************************************************************/

void mask(size_t length, const std::map<size_t, std::list<size_t>>& importances, size_t* masks)
{
    std::list<size_t> indicies;

    for (const auto& [importance, list] : importances)
    {
        if (indicies.size() + list.size() < length)
        {
            indicies.insert(indicies.end(), list.begin(), list.end());
        }
        else
        {
            indicies.insert(indicies.end(), list.begin(), std::next(list.begin(), length - indicies.size()));
            break;
        }
    }

    for (const auto& index : indicies)
    {
        masks[index] = 1;
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
	auto elites = selector_->sort(std::forward<Series>(individuals_));

	for (const auto& individual : *elites.begin())
	{
		for (size_t i = 0; i < scale_ + dimension_ + constraint_; ++i)
		{
			file << individual.first[i] << "\t";
		}
		file << std::endl;
	}

	file.close();
}

Population::Population(math::Optimizor::Configuration& configuration) :
    scale_(std::get<size_t>(configuration["scale"])),
    dimension_(std::get<size_t>(configuration["dimension"])),
    constraint_(std::get<size_t>(configuration["constraint"])),
    importances_(std::make_shared<std::map<size_t, std::list<size_t>>>()),
    selector_(std::make_unique<Reference>(configuration)), 
    reproducor_(std::make_unique<Reproducor>(importances_, configuration))
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

//  fill the initial population to the full size
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

//  determine the fitness of each decision variable
    importances(&(*initials.rbegin())[0], *configuration.objective);

    for(const auto& decisions : initials)
    {
        population_.push_back(create<double>(scale_ + dimension_ + constraint_));
        masks_.push_back(create<size_t>(scale_));

        auto&& individual = std::make_pair<>(population_.rbegin()->get(), masks_.rbegin()->get());

        math::copy(scale_, &decisions[0], 1, individual.first, 1);
        mask(std::floor(scale_ * 1 * uniform(generator)), *importances_, individual.second);

        evaluation(scale_, dimension_, *configuration.objective, individual);
        individuals_.push_back(individual);
    }
}