#include "unsga.h"

UNSGA::vector::vector(size_t length)
{
	address = (float*)mkl_malloc(length * sizeof(float), 64);
}

UNSGA::vector::~vector()
{
	mkl_free(address);
	address = nullptr;
}

UNSGA::UNSGA(const Configuration& configuration)
{
	configuration_ = std::make_shared<Configuration>(configuration);
	vslNewStream(&configuration_->stream, VSL_BRNG_MCG31, 1);

	reproducor_ = std::make_unique<Reproducor>(configuration_);

	reference_ = std::make_unique<Reference>(configuration_);
	
	configuration_->population = std::max(5 * reference_->size(), configuration_->population);
	population_ = std::make_unique<Population>(configuration_);
}

std::shared_ptr<Optimizer::Result> UNSGA::Optimize(Objective * objective)
{
	check(objective);

	for (size_t generation = 0; generation < configuration_->maximum; ++generation)
	{
		auto layers = population_->sort();
		auto selected = reference_->Select(layers);
		auto result = reproducor_->Reproduce(selected);
		population_->Update(result);
	}

	return results_;
}

void UNSGA::check(Objective* objective)
{
	bool population = false, plain = false;

	if (objective->scale() != configuration_->scale)
	{
		population = true;
		configuration_->scale = objective->scale();
	}

	if (objective->dimension() != configuration_->dimension)
	{
		plain = true;
		population = true;
		configuration_->dimension = objective->dimension();
	}

	configuration_->objective = objective;

	if (population)
	{
		population_.reset(new Population(configuration_));
	}

	if (plain)
	{
		reference_.reset(new Reference(configuration_));
	}
}

UNSGA::~UNSGA()
{
	vslDeleteStream(&configuration_->stream);
}