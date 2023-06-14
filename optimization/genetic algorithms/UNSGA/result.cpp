#include "unsga.h"

Result::Result(const Series<T>& elites) : elites_(elites)
{}

std::vector<std::vector<double>> Result::decisions() const 
{
	return {};
}

std::vector<std::vector<double>> Result::objectives() const
{
	return {};
}

void Result::Write(const char* filename) const
{
	std::ofstream file(filename);

	for (const auto& individual : elites_)
	{
		for (const auto& objective : individual->objectives)
		{
			file << objective << "\t";
		}
		file << std::endl;
	}

	file.close();
}