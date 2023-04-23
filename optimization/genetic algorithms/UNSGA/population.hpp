#include "individual.hpp"
#include "reproducer.hpp"
#include "reference plain.hpp"

#ifndef _MATH_OPTIMIZATION_UNSGA_POPULATION_
#define _MATH_OPTIMIZATION_UNSGA_POPULATION_
template<typename T>
class Population
{
private:
    Configuration<T>* configuration_;
	std::vector<std::unique_ptr<Individual<T>>> individuals_;
	Series<T> population_;

private:
    std::unique_ptr<Reference<T>> selector_;
    std::unique_ptr<Reproducor<T>> reproducer_;

public:
	Layer<T> sort() {
        Layer<T> results;

        for (auto individual = population_.begin(); individual != population_.end(); ++individual)
        {
            for (auto later = std::next(individual); later != population_.end(); ++later)
            {
                switch ((**individual) < (**later))
                {
                case 1 :
                {
                    (*individual)->dominates.push_back(*later);
                    (*later)->dominated++;
                    continue;
                }
                case -1 :
                {
                    (*later)->dominates.push_back(*individual);
                    (*individual)->dominated++;
                    continue;
                }
                };
            }
        }

        while (!population_.empty())
        {
            results.push_back({});
            population_.sort([](Individual<T>* lhs, Individual<T>* rhs) {return lhs->dominated < rhs->dominated; });

            auto location = population_.begin();
            for (location; location != population_.end(); ++location)
            {
                if ((*location)->dominated != 0)
                {
                    break;
                }
            }

            auto sequence = results.rbegin();
            (*sequence).splice((*sequence).end(), population_, population_.begin(), location);

            for (auto& individual : *results.rbegin())
            {
                for (auto& dominate : individual->dominates)
                {
                    dominate->dominated--;
                }

                individual->dominates.clear();
            }
        }

        return results;
    }

private:
    void Fitness(Individual<T>& individual) {
        configuration_->objective(&individual.decisions[0], &individual.objectives[0]);
        configuration_->constraint(&individual.decisions[0], &individual.objectives[0], &individual.voilations[0]);
    }

public:
    void Evolve() {
    }

public:
	Population(Configuration<T>* configuration) : configuration_(configuration) {
        individuals_.resize(configuration_->population);
        size_t count = 0;

        for(auto& individual : individuals_) {
            individual = std::make_unique<Individual<T>>(configuration_->initialization[count++]);
            Fitness(*individual);
        }
    }
};
#endif //!_MATH_OPTIMIZATION_UNSGA_POPULATION_