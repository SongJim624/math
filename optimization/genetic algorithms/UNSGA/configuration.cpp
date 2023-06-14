#include "unsga.h"

Configuration::Configuration(Optimizor::Configuration* configuration)
{
	dimensions = configuration->dimensions();
	scales = configuration->scales();
	constraints = configuration->constraints();

	uppers = std::vector<double>(configuration->uppers(), configuration->uppers() + scales);
	lowers = std::vector<double>(configuration->lowers(), configuration->lowers() + scales);
	integers = std::vector<double>(configuration->integers(), configuration->integers() + scales);

	objective = configuration->objective.get();
	constraint = configuration->constraint.get();

	maximum = std::get<size_t>((*configuration)["maximum"]);
	division = std::get<size_t>((*configuration)["division"]);
	population = std::get<size_t>((*configuration)["population"]);

	initialization.resize(population);
	for (size_t i = 0; i < population; ++i)
	{
		for(size_t j = 0; j< dimesion; ++j)
		{
			initialization[i] = uniform_(generator_) * (uppers[i] - lowers[i]) + lowers[i];
		}
	}

	cross = std::get<double>((*configuration)["cross"]);
	mutation = std::get<double>((*configuration)["mutation"]);
	threshold = 0.8;
}