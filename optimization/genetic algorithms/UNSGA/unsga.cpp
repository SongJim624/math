#include "unsga.h"

UNSGA::UNSGA() : configuration_(nullptr), population_(nullptr), results_(nullptr)
{
}

const Optimizor::Result* UNSGA::Optimize(Optimizor::Configuration* config)
{
	population_.reset(nullptr);
	Individual::dimensions = config->dimensions();
	Individual::scales = config->scales();
	Individual::constraints = config->constraints();

	std::unique_ptr<::Configuration> configuration = std::make_unique<::Configuration>(config);
	population_ = std::make_unique<Population>(configuration.get());

	for (size_t i = 0; i < configuration->maximum; ++i)
    {
		population_->Evolve();
	}

	results_ = std::make_unique<Result>(population_->Elite());
	return results_.get();
}