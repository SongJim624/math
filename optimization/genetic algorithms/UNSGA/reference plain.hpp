#include "configuration.hpp"
#include "individual.hpp"
#include "point.hpp"
#include <map>
#include <list>

#ifndef _MATH_OPTIMIZATION_UNSGA_REFERENCE_
#define _MATH_OPTIMIZATION_UNSGA_REFERENCE_
template<typename T>
class Reference
{
private:
using Cost = std::pair<std::map<Individual<T>*, Vector<T>*>, std::map<Individual<T>*, Vector<T>*>>;

private:
    size_t dimension_;
	std::list<std::unique_ptr<Point<T>>> points_;
    Vector<T> ideal_, interception_;
    std::vector<Vector<T>> costs_;

private:
//archievement scalar function
    T Scale(size_t dimension, const Vector<T>& objectives);
	void  Ideal(const Series<T>& individuals);
	void Interception(const Series<T>& individuals);

	Cost Normalize(const Series<T> solution, const Series<T>& critical);
	void Associate(const Cost& costs);
	void Dispense(size_t needed, Series<T>& solution, Series<T>& critical);

public:
    std::pair<Series<T>, Series<T>> Select(Layer<T> layers);
    Reference(Configuration<T>* configuration);
};

size_t Combination(size_t dimension, size_t division) {
    dimension += division - 1;
    division = std::min(division, dimension - division);
    size_t result = 1;

    for(size_t i = 0; i < division; ++i) {
        result *= dimension - i;
    }

    for(size_t i = 0; i < division; ++i) {
        result /= division - i;
    }

    return result;
}

template<typename T>
void Plain(size_t dimension, size_t division, std::vector<T*>& points) {
    if(dimension == 1) {
        points[0][0] = (T)division;
    }
    else {
        auto point = points.begin();

        for(size_t i = 0; i < division + 1; ++i) {
            size_t amount = Combination(dimension - 1, division - i);
            std::vector<T*> selected(amount, nullptr);

            for(size_t j = 0; j < amount; ++j) {
                (*point)[0] = (T)i;
                selected[j] = (*point) + 1;
                point++;
            }

            Plain(dimension - 1, division - i, selected);
        }
    }
}

template<typename T>
T Reference<T>::Scale(size_t dimension, const Vector<T>& objectives) {
    Vector<T> weights(dimension_, 1e-6);
    weights[dimension] = 1;

//further optimization opportunity
    for(size_t i = 0; i < dimension_; ++i) {
        weights[i] = objectives[i] / weights[i];
    }

    return *std::max_element(weights.begin(), weights.end());
}

template<typename T>
void  Reference<T>::Ideal(const Series<T>& individuals) {
    std::fill(ideal_.begin(), ideal_.end(), +INFINITY);
    for(const auto& individual : individuals)
    {
        for(size_t i = 0; i < dimension_; ++i)
        {
            ideal_[i] = std::min(ideal_[i], individual->objectives[i]);
        }
    }
}

template<typename T>
void Reference<T>::Interception(const Series<T>& individuals) {
    std::vector<std::map<T, Individual<T>*>> rank(dimension_);
    for (auto individual : individuals) {
        for (size_t objective = 0; objective < dimension_; ++objective) {
            rank[objective].insert({ Scale(objective, individual->objectives - ideal_), individual });
        }
    }

    std::vector<Vector<T>> matrix(dimension_);
    for(size_t i = 0; i < dimension_; ++i)
    {
        matrix[i] = rank[i].begin()->second->objectives - ideal_;
    }

    std::fill(interception_.begin(), interception_.end(), 1);
    Doolitle<T>(Matrix<T>(matrix), interception_);
}

template<typename T>
Reference<T>::Cost Reference<T>::Normalize(const Series<T> solution, const Series<T>& critical) {
    Ideal(solution);
    Interception(solution);

    Cost result({{}, {}});

    size_t count = 0;
    for(const auto& individual : solution)
    {
        auto& cost = costs_[count++];
        cost = (individual->objectives - ideal_) / interception_;
        result.first.insert({ individual, &cost });
    }

    for (const auto& individual : critical)
    {
        auto& cost = costs_[count++];
        cost = (individual->objectives - ideal_) / interception_;
        result.second.insert({ individual, &cost });
    }

    return result;
}

template<typename T>
void Reference<T>::Associate(const Cost& costs) {
    for (const auto& [individual, cost] : costs.first)
    {
        std::map<T, Point<T>*> rank;

        for (auto& point : points_) {
            T distance = point->distance(*cost);
            rank.insert({ distance, point.get() });
        }

        rank.begin()->second->count++;
    }

    std::map<Point<T>*, std::map<T, Individual<T>*>> association;

    for (const auto& [individual, cost] : costs.second)
    {
        std::map<T, Point<T>*> rank;

        for (auto& point : points_)
        {
            rank.insert({ point->distance(*cost), point.get()});
        }

        auto nearest = rank.begin();
        association[nearest->second].insert({ nearest->first, individual });
    }

    for (const auto& [point, associated] : association)
    {
        for (const auto& [distance, individual] : associated)
        {
            point->associated.push_back(individual);
        }
    }
}

template<typename T>
void Reference<T>::Dispense(size_t needed, Series<T>& solution, Series<T>& critical) {
    for (size_t i = 0; i < needed; ++i)
    {
        points_.sort([](std::unique_ptr<Point<T>>& lhs, std::unique_ptr<Point<T>>& rhs) {
            bool left = lhs->associated.empty();
            bool right = rhs->associated.empty();
            return left ? false : (right ? true : lhs->count < rhs->count);});

        solution.push_back(*(*points_.begin())->associated.begin());

        (*points_.begin())->associated.pop_front();
        (*points_.begin())->count++;
    }

    critical.clear();

    for (auto& point : points_)
    {
        point->count = 0;
        critical.splice(critical.end(), point->associated);
    }
}

template<typename T>
std::pair<Series<T>, Series<T>> Reference<T>::Select(Layer<T> layers) {
    std::pair<Series<T>, Series<T>> result{ {}, {} };

    size_t selection = 0;
    for(const auto& layer : layers) {
        selection += layer.size();
    }
    selection /= 2;

    auto& elites = result.first;

    //move the better individuals into the solution set
    while (!layers.empty())
    {
        if (elites.size() + layers.begin()->size() > selection)
        {
            break;
        }

        elites.splice(elites.end(), *layers.begin());
        layers.pop_front();
    }

    if (elites.empty()) 
    {
        elites.splice(elites.end(), *layers.begin());
        layers.pop_front();
    }

    //Niche technology needed
    if (selection > elites.size()) {
        Associate(Normalize(elites, *layers.begin()));
        Dispense(selection - elites.size(), elites, *layers.begin());
    }

    //nove the left one to the population for cross and mutation operation
    for (auto& layer : layers) 
    {
        result.second.splice(result.second.end(), layer);
    }

    return result;
}

template<typename T>
Reference<T>::Reference(Configuration<T>* configuration) {
    dimension_ = configuration->dimensions;
    size_t amount = Combination(dimension_, configuration->division);

    ideal_.resize(dimension_, +INFINITY);
    interception_.resize(dimension_, 1);
    costs_.resize(configuration->population, Vector<T>(dimension_));

    std::vector<T*> locations(amount, nullptr);
    for(size_t i = 0; i < amount; ++i){
        locations[i] = &costs_[i][0];
    }

    Plain(dimension_, configuration->division, locations);

    for (size_t i = 0; i < amount; ++i) {
        costs_[i] = (T) 1 / configuration->division * costs_[i];
        points_.push_back(std::make_unique<Point<T>>(costs_[i]));
    }
}
#endif //!_MATH_OPTIMIZATION_UNSGA_