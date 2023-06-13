#include "../../optimizer.h"
#include <time.h>
#include <list>
#include <map>
#include <algorithm>
#include <memory>
#include <array>
#include <cassert>

#ifndef _MATH_OPTIMIZATION_UNSGA_
#define _MATH_OPTIMIZATION_UNSGA_

class UNSGA : public Optimization::Optimizer
{
private:
	std::unique_ptr<Configuration> configuration_;
	std::unique_ptr<Population> population_;
	std::unique_ptr<Optimization::Result> results_;

public:
	UNSGA();
	virtual const Optimization::Result* Optimize(Optimization::Configuration* configuration);
};

UNSGA::UNSGA() : configuration_(nullptr), population_(nullptr), results_(nullptr)
{
}

template<typename T>
const Optimization::Result* UNSGA::Optimize(Optimization::Configuration* configuration)
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
#endif //!_MATH_OPTIMIZATION_UNSGA_