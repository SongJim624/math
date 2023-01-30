#include "unsga.h"

int compare(float lhs, float rhs)
{
	if(fabsf(lhs - rhs) < 1e-9)
	{
		return 0;
	}

	if(lhs > rhs)
	{
		return 1;
	}

	return -1;
}

int dominate(size_t size, const float * lhs, const float * rhs)
{
	size_t equal = 0, more = 0, less = 0;

	for(size_t i = 0; i < size; ++i)
	{
		switch(compare(lhs[i], rhs[i]))
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

	if(equal == size)
	{
		return 0;
	}

	if(more == 0)
	{
		return 1;
	}

	if(less == 0)
	{
		return -1;
	}

	return 0;
}

//Non dominated compare
void UNSGA::Dominate(size_t size, Individual* lhs, Individual* rhs)
{
	if((compare(lhs->voilation, 0) != 0) && (compare(rhs->voilation, 0) != 0))
	{
		return;
	}

	if((compare(lhs->voilation, 0) != 0))
	{
		rhs->dominates.push_back(rhs);
		lhs->dominated++;
		return;
	}

	if((compare(rhs->voilation, 0) != 0))
	{
		lhs->dominates.push_back(rhs);
		rhs->dominated++;
		return;
	}

	switch(dominate(size, lhs->objectives(), rhs->objectives()))
	{
	case 1:
	{
		lhs->dominates.push_back(rhs);
		rhs->dominated++;
		return;
	}
	case -1:
	{
		rhs->dominates.push_back(rhs);
		lhs->dominated++;
		return;
	}
	}
}

std::list<std::list<UNSGA::Individual*>> UNSGA::Sort(std::list<Individual*>& population)
{
	std::list<std::list<Individual*>> results;

	for(auto individual = population.begin(); individual != population.end(); ++individual)
	{
		for(auto later = std::next(individual); later != population.end(); ++later)
		{
			Dominate(information_->objective(), *individual, *later);
		}
	}

	while(!population.empty())
	{
		results.push_back({});
		population.sort([](Individual* lhs, Individual* rhs){
			lhs->dominated < rhs->dominated;
		});

		auto location = population.begin();
		for(location; location != population.end(); ++location)
		{
			if((*location)->dominated != 0)
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
	while(true)
	{
		if(solution.size() + layers.begin()->size() > selection)
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
