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
    size_t scale_;
    const T* uppers_, * lowers_;
    T cross_, mutation_, threshold_;
    const bool* integers_;

private:
    void Cross(std::array<const Individual<T>*, 2> parents, std::array<Individual<T>*, 2> children);
    void Mutate(Individual<T>* individual);
    void check(Individual<T>& individual);

public:
    Reproducor(Configuration<T>* configuration);
	//worse individuals in the children set would be replaced by children
   Series<T> Reproduce(std::pair<Series<T>, Series<T>> population);
};

template<typename T>
void Reproducor<T>::check(Individual<T>& individual)
{
	for (size_t i = 0; i < scale_; ++i) {
		individual.decisions[i] = std::max(std::min(individual.decisions[i], uppers_[i]), lowers_[i]);

		if (integers_[i]) {
			individual.decisions[i] = round(individual.decisions[i]);
		}
	}
}

template<typename T>
Reproducor<T>::Reproducor(Configuration<T>* configuration) {
    scale_ = configuration->scales;
    
    uppers_ = configuration->uppers;
    lowers_ = configuration->lowers;
    integers_ = configuration->integers;
   
    cross_ = configuration->cross;
    mutation_ = configuration->mutation;
    threshold_ = 0.8;
}

template<typename T>
void Reproducor<T>::Cross(std::array<const Individual<T>*, 2> parents, std::array<Individual<T>*, 2> children) {
    Vector<T> randoms = random<T>(scale_);

    for (size_t i = 0; i < scale_; ++i)
    {
        randoms[i] = (randoms[i] < 0.5) ? 2.0 * randoms[i] : 0.5 / (1.0 - randoms[i]);
    }

    randoms = randoms ^ (1 / (cross_ + 1));
    
    auto& father = parents[0]->decisions;
    auto& mother = parents[1]->decisions;

    auto& son = children[0]->decisions;
    auto& daughter = children[1]->decisions;

    son = 0.5 * (father + mother - randoms * (father - mother));
    daughter = 0.5 * (father + mother + randoms * (father - mother));
}

template<typename T>
void Reproducor<T>::Mutate(Individual<T>* individual) {
    Vector<T> randoms = random<T>(scale_);

    for (size_t i = 0; i < scale_; ++i) {
        auto r = randoms[i];
        auto& v = individual->decisions[i];

        if (randoms[i] < 0.5) {
            auto weight = (v - lowers_[i]) / (uppers_[i] - lowers_[i]);
            v = pow(2 * r + (1 - 2 * r) * pow(1 - weight, mutation_ + 1), 1 / (1 + mutation_)) - 1;
        }
        else {
            auto weight = (uppers_[i] - v) / (uppers_[i] - lowers_[i]);
            v = 1 - pow(2 * (1 - r) + (2 * r - 1) * pow(1 - weight, mutation_ + 1), 1 / (1 + mutation_));
        }
    }
}

template<typename T>
Series<T> Reproducor<T>::Reproduce(std::pair<Series<T>, Series<T>> population) {
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

        if (rand() /(T)RAND_MAX > threshold_) {
            Mutate(*son);
        }

        if (rand() / (T)RAND_MAX > threshold_) {
            Mutate(*daughter);
        }

        check(**son);
        check(**daughter);

        temporary.push_back(*son);
        temporary.push_back(*daughter);

        ordinary.pop_back();
        ordinary.pop_back();

        std::advance(father, 2);
        if (father == elites.end()) {
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
