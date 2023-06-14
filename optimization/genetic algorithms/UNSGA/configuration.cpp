#include "unsga.h"
#include <time.h>
#include <list>
#include <map>
#include <algorithm>
#include <memory>
#include <array>
#include <cassert>
#include "D:\\Windows\\Documents\\GitHub\\mathematical-tools\\basic\\eigen like\\math.hpp"

Configuration::Configuration(Optimizor::Configuration* configuration)
{
	dimensions = configuration->dimensions();
	scales = configuration->scales();
	constraints = configuration->constraints();

	uppers = configuration->uppers();
	lowers = configuration->lowers();
	integers = configuration->integers();

	objective = configuration->objective.get();
	constraint = configuration->constraint.get();

	const auto& config = *configuration;

	maximum = std::get<size_t>(config["maximum"]);
	division = std::get<size_t>(config["division"]);
	population = std::get<size_t>(config["population"]);

	initialization.resize(population);
	for (size_t i = 0; i < population; ++i)
	{
		initialization[i] = random<T>(scales, Vector<T>(scales, uppers), Vector<T>(scales, lowers));
	}

	cross = std::get<T>(config["cross"]);
	mutation = std::get<T>(config["mutation"]);
	threshold = 0.8;
}

Configuration::~Configuration()
{
}