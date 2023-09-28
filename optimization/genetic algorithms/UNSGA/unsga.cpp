#include "unsga.h"

Pointer create(size_t length)
{
    return Pointer(math::allocate<double>(length), math::free<double>);
}

void UNSGA::evolve(size_t generation)
{
	auto& individuals = population_->individuals;
	auto& selector = population_->selector();
	auto& reproducor = population_->reproducor();

    for (size_t i = 0; i < generation; ++i)
    {
        std::cout << "executing the " << i << " th generation" << std::endl;
        individuals = reproducor.reproduce(selector.select(individuals));
    }
}

void UNSGA::write(const char * filepath)
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

math::Optimizor::Result& UNSGA::optimize(math::Optimizor::Configuration& configuration)
{
	population_ = std::make_unique<Population>(configuration);
	evolve(std::get<size_t>(configuration["maximum"]));
	return *this;
}
