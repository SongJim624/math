#include "unsga.h"

UNSGA::UNSGA(const Configuration& configuration)
{
}

void UNSGA::Select(Genetic::Population* population)
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

void UNSGA::Reproduce(Genetic::Population* populaltion)
{
}

std::vector<std::vector<float>> UNSGA::Optimize(Information * information)
{
	Reference reference = Reference(information->objective(), configuration_->division);

	std::list<Individual*> population;
	std::list<Individual*> solution;

	for (size_t generation = 0; generation < configuration_->maximum; ++generation)
	{
		Select(solution, population);
		reproducor_->Reproduce(solution, population);

		population.splice(population.begin(), solution);
	}
}

UNSGA::~UNSGA()
{
}
