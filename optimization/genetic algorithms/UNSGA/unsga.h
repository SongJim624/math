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
template<typename T>
class Result : public Optimization::Result<T>
{
public:
	virtual std::vector<std::vector<T>> decisions() const {
		return {};
	}

	virtual std::vector<std::vector<T>> objectives() const {
		return{};
	}

	virtual void Write(const char* filename) const {
	}
};

template<typename T>
class UNSGA : public Optimization::Optimizer<T>
{
private:
	std::unique_ptr<Configuration<T>> configuraiton_;
	std::unique_ptr<Population<T>> population_;
	std::unique_ptr<Result<T>> results_;

public:
	UNSGA();

	virtual const Optimization::Result<T>* Optimize(Optimization::Configuration<T>* configuration) {
		if (configuraiton_) {
			configuraiton_->Update(configuration);
		}
		else {
			configuraiton_ = std::make_unique<Configuration<T>>(configuration);
		}
		
		for (size_t i = 0; i < configuraiton_->maximum; ++i) {
			population_->Evolve();
		}

		return results_.get();
	}
};

template<typename T>
UNSGA<T>::UNSGA(){
	results_ = std::make_unique<Result<T>>();
}
#endif //!_MATH_OPTIMIZATION_UNSGA_