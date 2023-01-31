#include "unsga.h"

std::list<std::list<UNSGA::Individual*>> UNSGA::Population::Sort(std::list<Individual*>& population)
{
	std::list<std::list<Individual*>> results;

	for (auto individual = population.cbegin(); individual != population.end(); ++individual)
	{
		for (auto later = std::next(individual); later != population.end(); ++later)
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

	while (!population.empty())
	{
		results.push_back({});
		population.sort([](Individual* lhs, Individual* rhs) {
			lhs->dominated < rhs->dominated;
			});

		auto location = population.begin();
		for (location; location != population.end(); ++location)
		{
			if ((*location)->dominated != 0)
			{
				break;
			}
		}

		auto sequence = results.rbegin();
		(*sequence).splice((*sequence).end(), population, population.begin(), location);

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

void UNSGA::Select(Reference& plain, std::list<Individual*>& population, std::list<Individual*>& solution)
{
	size_t selection = population.size() / 2;
	//after the sort operator, the population is an empty set
	std::list<std::list<Individual*>> layers = Sort(population);

	//move the better individuals into the solution set
	while (true)
	{
		if (solution.size() + layers.begin()->size() > selection)
		{
			break;
		}

		solution.splice(solution.end(), *layers.begin());
		layers.pop_front();
	}

	//Niche technology needed
	plain.Niche(selection - solution.size(), solution, *layers.begin());

	//nove the left one to the population for cross and mutation operation
	while (layers.size() != 0)
	{
		population.splice(population.end(), *layers.begin());
		layers.pop_front();
	}
}
