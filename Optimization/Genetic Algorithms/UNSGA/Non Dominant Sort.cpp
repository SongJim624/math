#include "UNSGA.h"
#include <map>
#include <algorithm>

int sgn(const float& x)
{
	return (0.0f < x) - (x < 0.0f);
}

//Non dominated compare
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