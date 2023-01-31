#include "unsga.h"

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
