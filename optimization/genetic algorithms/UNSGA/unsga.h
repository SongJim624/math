#include "../../optimizer.h"
//#include "../genetic algorithms.h"
#include <time.h>
#include <list>
#include <map>
#include <algorithm>
#include <memory>
#include <array>
#include <cassert>

#ifndef _MATH_OPTIMIZATION_UNSGA_
#define _MATH_OPTIMIZATION_UNSGA_

#include "configuration.hpp"
#include "population.hpp"
#include "reproducer.hpp"
#include "result.hpp"

template<typename T>
class UNSGA : public Optimization::Optimizer<T>
{
private:
	std::unique_ptr<Configuration<T>> configuration_;
	std::unique_ptr<Population<T>> population_;
	std::unique_ptr<Optimization::Result<T>> results_;

public:
	UNSGA();
	virtual const Optimization::Result<T>* Optimize(Optimization::Configuration<T>* configuration);
};

template<typename T>
UNSGA<T>::UNSGA() : configuration_(nullptr), population_(nullptr), results_(nullptr) {
}

template<typename T>
const Optimization::Result<T>* UNSGA<T>::Optimize(Optimization::Configuration<T>* configuration) {
	configuration_.reset(nullptr);
	population_.reset(nullptr);
	results_.reset(nullptr);

	srand((unsigned int)time(nullptr));

	Individual<T>::dimensions = configuration->dimensions();
	Individual<T>::scales = configuration->scales();
	Individual<T>::constraints = configuration->constraints();

	configuration_ = std::make_unique<Configuration<T>>(configuration);
	population_ = std::make_unique<Population<T>>(configuration_.get());

	for (size_t i = 0; i < configuration_->maximum; ++i) {
		population_->Evolve();
	}

	results_ = std::make_unique<Result<T>>(population_->Elite());
	return results_.get();
}
#endif //!_MATH_OPTIMIZATION_UNSGA_