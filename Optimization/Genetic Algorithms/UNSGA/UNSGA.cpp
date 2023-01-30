#include "unsga.h"

UNSGA::UNSGA(const Configuration& configuration)
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
