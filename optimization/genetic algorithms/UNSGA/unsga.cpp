#include "unsga.h"

UNSGA::UNSGA() : configuration_(nullptr), population_(nullptr), results_(nullptr)
{
}

const Optimizor::Result* UNSGA::Optimize(Optimizor::Configuration* configuration)
{
	configuration_.reset(nullptr);
	population_.reset(nullptr);
	results_.reset(nullptr);

	srand(0);

	Individual::dimensions = configuration->dimensions();
	Individual::scales = configuration->scales();
	Individual::constraints = configuration->constraints();

	configuration_ = std::make_unique<Configuration>(configuration);
	population_ = std::make_unique<Population>(configuration_.get());

	for (size_t i = 0; i < configuration_->maximum; ++i) {
		population_->Evolve();
	}

	results_ = std::make_unique<Result>(population_->Elite());
	return results_.get();
}