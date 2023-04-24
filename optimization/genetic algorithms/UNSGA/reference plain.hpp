#include "configuration.hpp"
#include "individual.hpp"
#include "point.hpp"

#ifndef _MATH_OPTIMIZATION_UNSGA_REFERENCE_
#define _MATH_OPTIMIZATION_UNSGA_REFERENCE_
template<typename T>
class Reference
{
private:
using Cost = std::pair<std::map<Individual<T>*, T*>, std::map<Individual<T>*, T*>> ;

private:
    int dimension_;
	std::list<std::unique_ptr<Point<T>>> points_;
    Vector<T> ideal_, interception_;
    std::vector<Vector<T>> costs_;

private:
//archievement scalar function
    T Scale(size_t dimension, const Vector<T>& objectives);
	void  Ideal(const std::list<Individual<T>*>& individuals);
	void Interception(const Series<T>& individuals);

	Cost Normalize(const std::list<Individual<T>*> solution, const std::list<Individual<T>*>& critical);
	void Associate(const Cost& costs);
	void Dispense(size_t needed, Series& solution, Series& critical);

public:
	Reference(Configuration<T>* configuration) : dimension_(configuration->dimension) {
        size_t amount = Combination(dimension_, configuration->division);

        ideal_.resize(dimension_, +INFINITY);
        interception_.resize(dimension_, 1);
        costs_.resize(amount * dimension_, 0);

        std::vector<T*> locations(amount, nullptr);
        for(size_t i = 0; i < amount; ++i){
            locations[i] = &costs_[i * dimension_];
        }

        Plain(dimension_, configuration->division, locations);

        for (size_t i = 0; i < amount; ++i)
        {
            cblas_sscal(dimension_, 1.0 / configuration->division, locations[i], 1);
            points_.push_back(std::make_unique<Point>(locations[i], dimension_));
        }
    }

public:
	std::pair<Series<T>, Series<T>> Select(Layer<T>& layers);
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
        points[0][0] = division;
    }
    else {
        for(size_t i = 0; i < division + 1; ++i) {
            size_t amount = Combination(dimension - 1, division - i);
            std::vector<T*> selected(amount);

            auto point = points.begin();
            for(size_t j = 0; j < amount; ++j) {
                (*point)[0] = i;
                selected[j] = (*point) + 1;
                point++;
            }

            Plain(dimension - 1, division - i, selected);
        }
    }
}

template<typename T>
T Reference<T>::Scale(size_t dimension, const Vector<T>& objectives) {
    Vector<T> weights(dimnesion_, 1e-6);
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
            ideal_[i] = fminf(ideal_[i], individual->objectives[i]);
        }
    }
}

template<typename T>
void Reference<T>::Interception(const Series<T>& individuals) {
    std::vector<std::map<T, Individual<T>*>> rank(dimension_);
    for (auto individual : individuals) {
        for (size_t objective = 0; objective < dimension_; ++objective) {
            auto temporary = individual->objectives - ideal_;
            rank[objective].insert({ Scale(objective, temporary), individual });
        }
    }

    Vector<T> matrix;
    for(size_t i = 0; i < dimension_; ++i)
    {
        auto temporary = rank[i].begin()->second->objectives - ideal_;
        matrix.insert(matrix.end(), temporary);
    }

    std::fill(interception_.begin(), interception_.end(), 1);
    std::vector<int>ipiv(dimension_);
    int info = 0, column = 1;
    T one = 1;

    if(std::is_same<T, float>) {
        sgesv(&dimension_, &column, &matrix[0], &dimension_, &ipiv[0], &interception_[0], &dimension_, &info);
        vsDivI(dimension_, &one, 0, &interception_[0], 1, &interception_[0], 1);
    }

    if(std::is_same<T, double>) {
        dgesv(&dimension_, &column, &matrix[0], &dimension_, &ipiv[0], &interception_[0], &dimension_, &info);
        vdDivI(dimension_, &one, 0, &interception_[0], 1, &interception_[0], 1);
    }
}

template<typename T>
Reference<T>::Cost Reference<T>::Normalize(const std::list<Individual<T>*> solution, const std::list<Individual<T>*>& critical) {
    Ideal(solution);
    Interception(solution);

    Cost result({{}, {}});
    size_t count = 0;

    auto normalize = [this](const std::vector<T, Allocator<T>>& objectives, T* cost) {
        (objectives - ideal) / interceptions
        return cost;
    };

    for(const auto& individual : solution)
    {
        T* cost = costs_[count * dimension_];
        result.first.insert({ individual, normalize(individual->objectives, cost) });
        count += dimension_;
    }

    for (const auto& individual : critical)
    {
        T* cost = costs_[count * dimension_];
        result.second.insert({ individual, normalize(individual->objectives, cost) });
        count += dimension_;
    }

    return result;
}

template<typename T>
void Reference<T>::Associate(const Cost& costs) {
    for (const auto& [individual, cost] : costs.first)
    {
        std::map<T, Point*> rank;

        for (auto& point : points_)
        {
            rank.insert({ point->distance(cost), point });
        }

        rank.begin()->second->count++;
    }

    std::map<Point*, std::map<T, Individual*>> association;

    for (const auto& [individual, cost] : costs.second)
    {
        std::map<float, Point*> rank;

        for (auto& point : points_)
        {
            rank.insert({ point->distance(cost), point });
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
        points_.sort([](std::unique_ptr<Point<T>> lhs, std::unique_ptr<Point<T>> rhs) {
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

        if (point->associated.empty())
        {
            continue;
        }
        else
        {
            critical.splice(critical.end(), point->associated);
        }
    }
}

template<typename T>
std::pair<Series<T>, Series<T>> Reference<T>::Select(Layer<T>& layers) {
    std::pair<Series<T>, Series<T>> result{ {}, {} };

    size_t selection = 0;
    for(const auto& layer : layers) {
        selection += layer.size();
    }
    selection /= 2;

    auto& elites = result.first;
    auto& ordinary = result.second;

    //move the better individuals into the solution set
    while (true) {
        if (elites.size() + layers.begin()->size() > selection) {
            break;
        }

        elites.splice(elites.end(), *layers.begin());
        layers.pop_front();
    }

    if (elites.empty()) {
        elites.splice(elites.end(), *layers.begin());
        layers.pop_front();
    }

    //Niche technology needed
    if (selection > elites.size()) {
        Associate(Normalize(elites, *layers.begin()));
        Dispense(selection - elites.szie(), elites, *layers.begin());
    }

    //nove the left one to the population for cross and mutation operation
    while (layers.size() != 0)
    {
        ordinary.splice(ordinary.end(), *layers.begin());
        layers.pop_front();
    }

    return result;
}

template<typename T>
Reference<T>::Reference(Configuration<T>* configuration) {
    dimension_ = configuration->dimension;
    size_t amount = Combination(dimension_, configuration->division);

    ideal_.resize(dimension_, +INFINITY);
    interception_.resize(dimension_, 1);
    costs_.resize(amount * dimension_, 0);

    std::vector<T*> locations(amount, nullptr);
    for(size_t i = 0; i < amount; ++i){
        locations[i] = &costs_[i * dimension_];
    }

    Plain(dimension_, configuration->division, locations);

    for (size_t i = 0; i < amount; ++i) {
        costs_[i] *= (T) 1 / configuraiton->division;
        points_.push_back(std::make_unique<Point>(costs_[i]));
    }
}
#endif //!_MATH_OPTIMIZATION_UNSGA_