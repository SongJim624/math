#include <time.h>
#include <array>
#include "D:\\Windows\\Documents\\GitHub\\mathematical-tools\\basic\\eigen like\\vector.hpp"
#include "individual.hpp"
#include "configuration.hpp"

#ifndef _MATH_OPTIMIZATION_UNSGA_REPRODUCER_
#define _MATH_OPTIMIZATION_UNSGA_REPRODUCER_
template<typename T>
class Reproducor
{
private:
    size_t dimension_, scale_, constraints_;
    T *uppers_, lowers_, cross_, mutation_;
    bool* integers_;

private:
    void Cross(std::array<const Individual<T>*, 2> parents, std::array<Individual<T>*, 2> children);
    void Mutate(Individual<T>* individual);
    void check(Individual<T>& individual);

public:
    Reproducor(Configuration<T>* configuration);
	//worse individuals in the children set would be replaced by children
   Series<T> Reproduce(std::pair<Series<T>, Series<T>>& population);
};

//function to generate a random vector within 0-1
template<typename T>
Vector<T> random(size_t length) {
    Vector<T> results(length);

    for (size_t i = 0; i < length; ++i) {
        results[i] = rand() / (T) RAND_MAX;
    }

    return results;
}

template<typename T>
void Reproducor<T>::check(Individual<T>& individual)
{
	for (size_t i = 0; i < dimension_; ++i) {
		individual.decisions[i] = std::max(std::min(individual.decisions[i], uppers_), lowers_);

		if (integers_[i]) {
			individual.decisions[i] = std::round(individual.decisions[i]);
		}
	}
}

template<typename T>
Reproducor<T>::Reproducor(Configuration<T>* configuration) {
    cross_ = configuration->cross;
    mutation_ = configuration->mutation;
    srand((unsigned)time(nullptr));
}

template<typename T>
void Reproducor<T>::Cross(std::array<const Individual<T>*, 2> parents, std::array<Individual<T>*, 2> children) {
    Vector<T> randoms(scale_); //= random((size_t)scale_);

    for (size_t i = 0; i < scale_; ++i)
    {
        randoms[i] = (randoms[i] < 0.5) ? 2.0 * randoms[i] : 0.5 / (1.0 - randoms[i]);
    }

    /*
    power(1 / (configuration_->cross + 1), randoms_);

    add(1, randoms_, weights_);
    multiply(weights_, father.decisions, son.decisions);
    substract(1, randoms_, weights_);
    multiply(weights_, mother.decisions, weights_);
    add(weights_, son.decisions, son.decisions);

    substract(1, randoms_, weights_);
    multiply(weights_, father.decisions, daughter.decisions);
    add(1, randoms_, weights_);
    multiply(weights_, mother.decisions, weights_);
    add(weights_, daughter.decisions, son.decisions);

    cblas_sscal(scale_, 0.5, &son.decisions[0], 1);
    cblas_sscal(scale_, 0.5, &daughter.decisions[0], 1);
    */
}

template<typename T>
void Reproducor<T>::Mutate(Individual<T>* individual) {
   /*
    std::vector<bool> labels(scale_, false);

    Random();

    for (size_t i = 0; i < scale_; ++i) {
        if (randoms_[i] > 0.5) {
            randoms_[i] = 1 - randoms_[i];
            labels[i] = true;
        }
    }

    cblas_sscal(scale_, 2, &randoms_[0], 1);
    power(1 / (configuration_->mutation + 1), randoms_);
    add(-1, randoms_, randoms_);

    for (size_t i = 0; i < scale_; ++i)
    {
        if (labels[i])
        {
            randoms_[i] = -randoms_[i];
        }
    }

    substract(configuration_->upper(), configuration_->lower(), individual.decisions);
    multiply(randoms_, individual.decisions, randoms_);
    add(randoms_, configuration_->lower(), individual.decisions);
    */
}

template<typename T>
Series<T> Reproducor<T>::Reproduce(std::pair<Series<T>, Series<T>>& population) {
    Series<T> result, temporary;

    auto& elites = population.first;
    auto& ordinary = population.second;

    if (elites.size() % 2)
    {
        ordinary.push_front(*elites.rbegin());
        elites.pop_back();
    }

    auto father = elites.begin();
    auto mother = std::next(father);

    while (true)
    {
        if (ordinary.empty())
        {
            break;
        }

        auto son = ordinary.rbegin();
        auto daughter = std::next(son);

        Cross({ *father, *mother }, { *son, *daughter });

        Vector<T> randoms(2);

        T threshold = 1; //configuration->threshold
        if (randoms[0] > threshold) {
            Mutate(*son);
        }

        if (randoms[1] > threshold) {
            Mutate(*daughter);
        }

        temporary.push_back(*son);
        temporary.push_back(*daughter);

        ordinary.pop_back();
        ordinary.pop_back();

        std::advance(father, 2);
        if (father == elites.end())
        {
            break;
        }

        std::advance(mother, 2);
    }

    result.splice(result.end(), elites);
    result.splice(result.end(), temporary);
    result.splice(result.end(), ordinary);
    return result;
}

#endif //!_MATH_OPTIMIZATION_UNSGA_REPRODUCER_
