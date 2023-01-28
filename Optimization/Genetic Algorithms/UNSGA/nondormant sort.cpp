#include "UNSGA.h"
#include <map>
#include <algorithm>

int Compare(float lhs, float rhs)
{
	if(fabsf(lhs, rhs) < 1e-9)
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


void Compare(Individual_UNSGA* A, Individual_UNSGA* B)
{
	if (A->individual.voilate * B->individual.voilate != 0)
	{
		//Non dominate each other
		return;
	}

	if (B->individual.voilate != 0)
	{
		A->dominate.push_back(B);
		B->dominated++;
		return;
	}

	if (A->individual.voilate != 0)
	{
		B->dominate.push_back(A);
		A->dominated++;
		return;
	}

	long more = 0, less = 0, equal = 0;


	for (long i = 0; i < Individual::objective_size; ++i)
	{
		A->individual.objectives[i] == B->individual.objectives[i] ? equal++ :
			A->individual.objectives[i] > B->individual.objectives[i] ? more++ : less++;
	}

	if (equal == Individual::objective_size)
	{
		return;
	}

	//A is dominated by B
	if (less == 0)
	{
		B->dominate.push_back(A);
		A->dominated++;
		return;
	}

	//A dominate B
	if (more == 0)
	{
		A->dominate.push_back(B);
		B->dominated++;
		return;
	}
}

std::list<std::list<Individual*>> UNSGA::Sort(std::list<Individual*> population)
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

std::list<std::list<Individual_UNSGA*>> UNSGA::Sort()
{
	std::list<Individual_UNSGA*> relations{};

	for (long i = 0; i < total_size; ++i)
	{
		relations.push_back(individuals[i]);

		for (long j = i + 1; j < total_size; ++j)
		{
			Compare(individuals[i], individuals[j]);
		}
	}

	std::list<std::list<Individual_UNSGA*>> layers = {};

	while (relations.size() != 0)
	{
		relations.sort(DominationCMP());

		layers.push_back({});

		while (relations.size() != 0 && (*relations.begin())->dominated == 0)
		{
			layers.rbegin()->push_back(*relations.begin());
			relations.pop_front();
		}

		for (auto iter : *layers.rbegin())
		{
			for (auto it : iter->dominate)
			{
				it->dominated--;
			}

			iter->dominate.clear();
		}
	}

	return layers;
}