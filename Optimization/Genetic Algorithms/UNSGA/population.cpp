#include "unsga.h"

UNSGA::Population::Population(std::shared_ptr<Configuration> configuration)
	: configuration_(configuration)
{
	for (const auto& decisions : configuration->initialization)
	{
		population_.push_back(new Individual(configuration, &decisions[0]));
	}

	for (size_t i = 0; i < configuration->population - configuration->initialization.size(); ++i)
	{
		population_.push_back(new Individual(configuration));
	}
}

UNSGA::Population::~Population()
{
	for (auto& individual : population_)
	{
		delete individual;
		individual = nullptr;
	}
}

void UNSGA::Population::fitness(Individual* individual)
{
	configuration_->objective->function(individual->decisions, individual->objectives, &individual->penalty);
}

std::list<std::list<UNSGA::Individual*>> UNSGA::Population::sort()
{
	for (auto individual : population_)
	{
		fitness(individual);
	}

	std::list<std::list<Individual*>> results;

	for (auto individual = population_.begin(); individual != population_.end(); ++individual)
	{
		for (auto later = std::next(individual); later != population_.end(); ++later)
		{
			switch ((*individual) < (*later))
			{
			case 1 :
			{
				(*individual)->dominates.push_back(*later);
				(*later)->dominated++;
				continue;
			}
			case -1 :
			{
				(*later)->dominates.push_back(*individual);
				(*individual)->dominated++;
				continue;
			}
			};
		}
	}

	while (!population_.empty())
	{
		results.push_back({});
		population_.sort([](Individual* lhs, Individual* rhs) {
			return lhs->dominated < rhs->dominated;
			});

		auto location = population_.begin();
		for (location; location != population_.end(); ++location)
		{
			if ((*location)->dominated != 0)
			{
				break;
			}
		}

		auto sequence = results.rbegin();
		(*sequence).splice((*sequence).end(), population_, population_.begin(), location);

		for (auto& individual : *results.rbegin())
		{
			for (auto& dominate : individual->dominates)
			{
				dominate->dominated--;
			}

			individual->dominates.clear();
		}
	}

	return results;
};

void UNSGA::Population::Update(std::list<Individual*>& population)
{
	population_ = std::move(population);
}