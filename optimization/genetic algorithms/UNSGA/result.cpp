
#include "population.h"


std::vector<std::vector<T, allocator>> UNSGA<T, allocator>::Population::decisions() const
{
	return {};
}

std::vector<std::vector<T, allocator>> UNSGA<T, allocator>::Population::objectives() const
{
	return {};
}

template<typename T, class allocator>
void UNSGA<T, allocator>::Population::Write(const char* filename) const
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
#endif //! _MATH_OPTIMIZTION_UNSGA_RESULTS_