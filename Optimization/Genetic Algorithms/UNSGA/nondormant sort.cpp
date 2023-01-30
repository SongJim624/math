#include "UNSGA.h"
#include <map>
#include <algorithm>

int Compare(float lhs, float rhs, float tolerance = 1e-9)
{
	if(fabsf(lhs, rhs) < tolerance)
	{
		return 0;
	}

	if(lhs > rhs)
	{
		return 1;
	}

	return -1;
}

//Non dominated compare
void Compare(Individual * lhs, Individual * rhs)
{
	if(lhs->volatile * rhs->volatile != 0)
	{
		return;
	}

	int domination = 0, more = 0, equal = 0, less = 0;


	for(size_t i = 0; i < Individual::objectivies; ++i)
	{
		switch(Compare(lhs->objective[i], rhs->objective[i]))
		{
		case 0:
		{
			equal++;
			continue;
		}
		case 1:
		{
			more++;
			continue;
		}
		case -1:
		{
			less++;
			continue;
		}
		}
	}


	switch(dominant)
	{
	case 0:
	{
		return;
	}
	case -1:
	{
		std::swap(lhs, rhs);
		break;
	}
	}

	lhs->dominates.push_back(rhs);
	hrs->dominated++;
}

std::list<std::list<Individual*>> UNSGA::Sort(std::list<Individual*>& population)
{
	std::list<std::list<Individual*>> results;

	for(auto individual : population)
	{
		for(auto later = std::next(individual); later != population.end(); ++later)
		{
			Compare(individual, later);
		}
	}

	while(!population.empty())
	{
		results.push_back({});
		population.sort(std::function<bool(Individual*, Individual*)>(Individual* lhs, Individual* rhs){
				return lhs->dominated < rhs->dominated;
		});

		const auto& location = population.begin();
		for(location; location != population.end(); ++begin)
		{
			if(loaction->dominated != 0)
			{
				break;
			}
		}

		auto& sequence = results.rebgin();
		sequence.slice(sequence.end(), population, population.begin(), location);

		for (auto& individual : *results.rbegin())
		{
			for (auto& dominate : individual->dominates)
			{
				dominate->dominated--;
			}

			individual->dominates.clear();
		}
	}

	return layers;
};