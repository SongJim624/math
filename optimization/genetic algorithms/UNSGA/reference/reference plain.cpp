#include "unsga.h"

double Reference::Scale(size_t dimension, const std::vector<double>& objectives)
{
    std::vector<double> weights(dimension_, 1e-6);
    weights[dimension] = 1;

//further optimization opportunity
    for(size_t i = 0; i < dimension_; ++i)
    {
        weights[i] = objectives[i] / weights[i];
    }

    return *std::max_element(weights.begin(), weights.end());
}

void  Reference::Ideal(const Reference::Series& individuals)
{
    std::fill(ideals_.begin(), ideals_.end(), +INFINITY);
    for(const auto& individual : individuals)
    {
        for(size_t i = 0; i < dimension_; ++i)
        {
            ideals_[i] = std::min(ideals_[i], individual->objectives[i]);
        }
    }
}

void Reference::Interception(const Reference::Series& individuals)
{
    std::vector<std::map<double, Individual*>> rank(dimension_);
    for (auto individual : individuals)
    {
        for (size_t objective = 0; objective < dimension_; ++objective)
        {
            rank[objective].insert({ Scale(objective, individual->objectives - ideal_), individual });
        }
    }

    std::vector<std::vector<double>> matrix(dimension_);
    for(size_t i = 0; i < dimension_; ++i)
    {
        matrix[i] = rank[i].begin()->second->objectives - ideal_;
    }

    std::fill(interceptions_.begin(), interceptions_.end(), 1);
    Doolitle(matrix, interceptions_);

    /*
    for (const auto& interception : interception_)
    {
        if (std::isnan(interception))
        {
            interception_ = worst;
            break;
        }
    }
    */
}

void Reference::Normalize(Reference::Series individuals)
{
    for(const auto& individual : individuals)
    {
        individual->objectives = (individual->objectives - ideal_) / interception_;
    }
}

void Reference::Associate(Reference::Series solution, Reference::Series& critical)
{
    for (const auto& individual : solution)
    {
        std::map<double, Point*> rank;

        for (auto& point : points_)
        {
            rank.insert({ point->distance(individual->objectives), point.get() });
        }

        rank.begin()->second->count++;
    }

    std::map<Point*, std::map<double, Individual*>> association;

    for (const auto& individual : critical)
    {
        std::map<double, Point*> rank;

        for (auto& point : points_)
        {
            rank.insert({ point->distance(individual->objectives), point.get()});
        }

        auto nearest = rank.begin();
        nearest->second->associated.push_back(individual);

//        association[nearest->second].insert({ nearest->first, individual });
    }
    critical.clear();

    /*
    for (const auto& [point, associated] : association)
    {
        for (const auto& [distance, individual] : associated)
        {
            point->associated.push_back(individual);
        }
    }
    */
}

void Reference::Dispense(size_t needed, Series& solution, Series& critical) {
    for (size_t i = 0; i < needed; ++i)
    {
        points_.sort([](std::unique_ptr<Point>& lhs, std::unique_ptr<Point>& rhs) {
            bool left = lhs->associated.empty();
            bool right = rhs->associated.empty();
            return left ? false : (right ? true : lhs->count < rhs->count);});

        solution.push_back(*(*points_.begin())->associated.begin());

        (*points_.begin())->associated.pop_front();
        (*points_.begin())->count++;
    }

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

    if (layers.begin()->size() >= selection)
    {
        auto first = layers.begin();
        result.first.splice(result.first.end(), *first, first->begin(), std::next(first->begin(), selection));
    }
    else
    {
//        auto& elites = result.first;

        //move the better individuals into the solution set
        while (!layers.empty())
        {
            if (result.first.size() + layers.begin()->size() > selection)
            {
                break;
            }

            result.first.splice(result.first.end(), *layers.begin());
            layers.pop_front();
        }

        //Niche technology needed
        if (selection > result.first.size())
        {
            Ideal(result.first);
            Interception(result.first);

            Normalize(result.first);
            Normalize(*layers.begin());

            Associate(result.first, *layers.begin());
            Dispense(selection - result.first.size(), result.first, *layers.begin());
        }
    }

    //nove the left one to the population for cross and mutation operation
    for (auto& layer : layers)
    {
        result.second.splice(result.second.end(), layer);
    }

    return result;
}

Reference::Reference(Configuration* configuration) :
    dimension_(configuration->dimensions),
    ideals_(std::vector<double>(dimension_, +INFINITY)),
    interceptions_(std::vector<double>(dimension_, 1))
{
    std::vector<double> location(configuration->division);
    std::iota(location.begin(), location.end(), 0);
    location /= configuration->division;

    while(std::next_permutation(location.begin(), location.end()))
    {
        points_.push_back(std::make_unique<Point>(location));
    }
}