#include "unsga.h"

extern "C" EXPORT void* create()
{
	return new UNSGA();
}

void UNSGA::evolve(size_t generation)
{
	auto& individuals = population_->individuals;
	auto& selector = population_->selector();
	auto& reproducor = population_->reproducor();

    for (size_t i = 0; i < generation; ++i)
    {
        individuals = reproducor.reproduce(selector.select(individuals));
    }
}

void UNSGA::write(const char * filepath, char mode)
{
	std::ofstream file(filepath);

	auto& individuals = population_->individuals;
	auto& selector = population_->selector();

	auto&& layers = selector.sort(individuals);
	for (const auto& individual : *layers.begin())
//	for (const auto& individual : individuals_)
	{
		for (size_t i = 0; i < population_->scale; ++i)
		{
			file << individual->decisions[i] << "\t";
		}

		for (size_t i = 0; i < population_->dimension; ++i)
		{
			file << individual->objectives[i] << "\t";
		}

		for (size_t i = 0; i < population_->constraint; ++i)
		{
			file << individual->voilations[i] << "\t";
		}

		file << std::endl;
	}

	file.close();
}

std::list<std::shared_ptr<const double[]>> UNSGA::results()
{
	elites_.clear();

	auto& individuals = population_->individuals;
	auto& selector = population_->selector();

	auto&& layers = selector.sort(individuals);

	for (auto& individual : *layers.begin())
	{
		elites_.emplace_back(std::shared_ptr<const double[]>(individual->decisions, [](void*) {}));
	}

	return elites_;
}


math::Optimizor::Result& UNSGA::optimize(math::Optimizor::Configuration& configuration)
{
	population_ = std::make_unique<Population>(configuration);
	evolve(std::get<size_t>(configuration["maximum"]));
	return *this;
}