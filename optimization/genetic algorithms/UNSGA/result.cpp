#include "unsga.h"

void UNSGA::Population::Write(const char* filename) const
{

	std::ofstream file(filename);
	auto elites = *sort(individuals_).begin();

	for (const auto& individual : elites)
	{
		for (size_t i = 0; i < scale_ + dimension_ + constraint_; ++i)
		{
			file << individual[i] << "\t";
		}
		file << std::endl;
	}

	file.close();
}