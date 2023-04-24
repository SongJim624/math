#include "../../optimizer.h"
//#include "../genetic algorithms.h"
#include <time.h>
#include <list>
#include <map>
#include <algorithm>
#include <memory>
#include <array>
#include <mkl.h>
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
	std::unique_ptr<Configuration<T>> configuraiton_;
	std::unique_ptr<Population<T>> population_;
	std::unique_ptr<Result<T>> results_;

public:
	UNSGA();
	virtual const Optimization::Result<T>* Optimize(Optimization::Configuration<T>* configuration);
};

template<typename T>
UNSGA<T>::UNSGA() : configuration_(nullptr), population_(nullptr) {
}

template<typename T>
const Optimization::Result<T>* NSGA<T>::Optimize(Optimization::Configuration<T>* configuration) {
	configuration_.reset(std::make_unique<Configuration<T>>(configuration));
	population_.reset(std::make_unique<Population<T>>(configuration_));

	for (size_t i = 0; i < configuraiton_->maximum; ++i) {
		population_->Evolve();
	}

	return results_.get();
}
#endif //!_MATH_OPTIMIZATION_UNSGA_