#include "unsga.h"

//a scalar function
double scale(size_t position, size_t dimension, const double * objective)
{
    auto weights = std::unique_ptr<double[], decltype(&math::free)>{math::allocate(dimension), math::free};

//    double * weights = math::allocate(dimension, 1e-6);
    weights[position] = 1;

    math::div(dimension, objective, weights, weights);
    return *std::max_element(weights.get(), weights.get() + dimension);
}

void math::UNSGA::Population::Reference::Interception(const Series& individuals)
{
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

void UNSGA::Population::Reference::normalize(Series& elites, Series& criticals)
{
    auto ideal = [this](Series individuals)
    {
        std::vector<double> ideal_(dimension_, +INFINITY);

        for(const auto& individual : individuals)
        {
            for(size_t i = 0; i < dimension_; ++i)
            {
                ideal_[i] = std::min(ideal_[i], individual->objectives[i]);
            }
        }

        return ideal_;
    };

    std::vector<double> ideal_ = ideal();


    auto interception = [this, ideal_](Series individuals)
    {
        std::vector<std::map<double, Individual*>> rank(dimension_);
        for (auto individual : individuals)
        {
            for (size_t objective = 0; objective < dimension_; ++objective)
            {
                rank[objective].insert({ Scale(objective, individual->objectives - ideal_), individual });
            }
        }

        std::vector<double> matrix(dimension_ * dimension_);

        for(size_t i = 0; i < dimension_; ++i)
        {
            matrix[i] = rank[i].begin()->second->objectives - ideal_;
        }

        std::vector<double> interception_(dimension_, 1);
        solve(matrix, interception_);
        return interception_;
    };

    std::vector<double> interception_ = interception(elites);

    auto normalization = [ideal_, interception_](Series& individuals)
    {
        for(const auto& individual : individuals)
        {
            individual->objectives = (individual->objectives - ideal_) / interception_;
        }
    };

    normalization(elites);
    normalization(criticals);
}

void Reference::Associate(Series solution, Series& critical)
{
    for (const auto& individual : solution)
    {
        std::map<double, Point<double>*> rank;

        for (auto& point : points_)
        {
            double distance = point->distance(individual->objectives);
            rank.insert({ distance, point.get() });
        }

        rank.begin()->second->count++;
    }

    std::map<Point*, std::map<double, Individual*>> association;

    for (const auto& individual : critical)
    {
        std::map<double, Point<double>*> rank;

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

template<typename double>
void Reference<double>::Dispense(size_t needed, Series<double>& solution, Series<double>& critical) {
    for (size_t i = 0; i < needed; ++i)
    {
        points_.sort([](std::unique_ptr<Point<double>>& lhs, std::unique_ptr<Point<double>>& rhs) {
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


std::pair<Series, Series> UNSGA::Population::Reference::select(std::list<Series> layers)
{
    std::pair<Series<double>, Series<double>> result{ {}, {} };

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

Reference::Reference(Optimizor::Configuration* configuration) :
    dimension_(std::get<size_t>(*(configuration)("dimension")))
    ideal_(allocate(dimension_)),
    interception_(allocate(dimension_ * dimension_))
{
    std::iota(ideal_, ideal_ + dimension);
    scal(dimension_, 1 / dimension_, ideal_, 1);

    while(std::next_permutation(ideal_, ideal_ + dimension))
    {
        points_.push_back(std::make_unique<Point>(dimension_, ideal_));
    }
}
