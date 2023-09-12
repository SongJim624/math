#include "unsga.h"
#include <iostream>

void doolittle(size_t dimension, double* matrix, double* vector)
{
    auto temporary = std::unique_ptr<double[], decltype(&math::free)>{ math::allocate(dimension * dimension), math::free };

    size_t row = dimension;
    size_t column = dimension;
    size_t scale = dimension;

    for (size_t r = 1; r < row; ++r)
    {
        temporary[r * column] /= matrix[r * column];
    }

    for (size_t k = 1; k < scale; ++k)
    {
        //update the upper triangle matrix
        for (size_t c = k; c < column; ++c)
        {
            double sum = 0;
            for (size_t t = 0; t < k; ++t)
            {
                sum += temporary[k * column + t] * temporary[t * column + c];
            }

            temporary[k * column + c] = matrix[k * column + c] - sum;
        }

        //update the lower triangle matrix
        for (size_t r = k + 1; r < row; ++r)
        {
            double sum = 0;
            for (size_t t = 0; t < k; ++t)
            {
                sum += temporary[r * column + t] * temporary[t * column + k];
            }

            temporary[r * column + k] = (matrix[r * column + k] - sum) / temporary[k * column + k];
        }
    }

    //solve the lower triangle
    for (size_t i = 1; i < scale; ++i)
    {
        double sum = 0;
        for (size_t k = 0; k < i; ++k)
        {
            sum += temporary[i * column + k] * vector[k];
        }

        vector[i] -= sum;
    }

    //solve the upper triangle
    vector[row - 1] /= temporary[(row - 1) * column + (column - 1)];

    for (int r = row - 2; r >= 0; --r)
    {
        double sum = 0;
        for (size_t k = r + 1; k < row; ++k)
        {
            sum += temporary[r * column + k] * vector[k];
        }

        vector[r] = (vector[r] - sum) / temporary[r * column + r];
    }
}

//a scalar function
double scale(size_t position, size_t dimension, const double * objective)
{
    auto weights = std::unique_ptr<double[], decltype(&math::free)>{math::allocate(dimension), math::free};
    weights[position] = 1;

    math::div(dimension, objective, weights.get(), weights.get());
    return *std::max_element(weights.get(), weights.get() + dimension);
}

double* ideal(double* point, size_t scale, size_t dimension, const std::list<double*>& individuals)
{
    std::fill(point, point + dimension, double(+INFINITY));

    for (const auto& individual : individuals)
    {
        for (size_t i = 0; i < dimension; ++i)
        {
            point[i] = std::min(point[i], individual[scale + i]);
        }
    }

    return point;
}

double* interception(double* values, const double * ideal, size_t scale, size_t dimension, const std::list<double*>& individuals)
{
    auto cost = std::unique_ptr<double[], decltype(&math::free)>{ math::allocate(dimension), math::free };
    auto matrix = std::unique_ptr<double[], decltype(&math::free)>{ math::allocate(dimension * dimension), math::free };
    std::vector<std::pair<double, double*>> nearest(dimension, { double(+INFINITY), nullptr});

    for (auto& individual : individuals)
    {
        math::sub(dimension, individual + scale, ideal, cost.get());

        for (size_t axis = 0;  axis < dimension; ++axis)
        {
            double distance = ::scale(axis, dimension, cost.get());

            if (distance < nearest[axis].first)
            {
                nearest[axis].first = distance;
                nearest[axis].second = individual;
            }
        }
    }

    std::fill(values, values + dimension, 1);
    for (size_t i = 0; i < dimension; ++i)
    {
        math::copy(dimension, nearest[i].second, 1, matrix.get() + i * dimension, 1);
    }

    doolittle(dimension, matrix.get(), values);
    return values;
}

double* normalize(size_t dimension, double* objectives, const double* ideal, const double* interception)
{
    math::sub(dimension, objectives, ideal, objectives);
    math::div(dimension, objectives, interception, objectives);
    return objectives;
}


std::list<std::list<double>> permutation(size_t dimension, size_t division)
{
    if (dimension == 1)
    {
        return { { (double)division } };
    }

    std::list<std::list<double>> results;

    for (size_t i = 0; i <= division; ++i)
    {
        std::list<std::list<double>> res = permutation(dimension - 1, division - i);

        for (auto& result : res)
        {
            result.push_front(i);
        }

        results.merge(res);
    }

    return results;
}

UNSGA::Population::Reference::Reference(const Optimizor::Configuration& configuration) :
    scale_(std::get<size_t>(configuration["scale"])),
    dimension_(std::get<size_t>(configuration["dimension"])),
    ideal_(math::allocate(dimension_)),
    interception_(math::allocate(dimension_)),
    selection_(std::get<size_t>(configuration["population"]) / 2)
{
    size_t division = std::get<size_t>(configuration["division"]);

    for (const auto& point : permutation(dimension_, division))
    {
        std::copy(point.begin(), point.end(), ideal_);
        math::scal(dimension_, 1.0 / division, ideal_, 1);
        points_.push_back(std::make_unique<Point>(dimension_, ideal_));
    }
}

void UNSGA::Population::Reference::attach(const double * cost)
{
    std::pair<double, Point*> nearest {double(+INFINITY), nullptr};

    for (const auto& point : points_)
    {
        double distance = point->distance(cost);
//        std::cout << distance << std::endl;
        if (distance < nearest.first)
        {
            nearest.first = distance;
            nearest.second = point.get();
        }
    }

    nearest.second->count++;
}

void UNSGA::Population::Reference::associate(double* individual, double * cost)
{
    std::pair<double, Point*> nearest {+INFINITY, nullptr};

    for (const auto& point : points_)
    {
        double distance = point->distance(cost);

        if (distance < nearest.first)
        {
            nearest.first = distance;
            nearest.second = point.get();
        }
    }

    nearest.second->associated.push_back(individual);
}

void UNSGA::Population::Reference::dispense(size_t needed, std::list<double*>& elites, std::list<double*>& critical)
{
    ideal_ = ideal(ideal_, scale_, dimension_, elites);
    interception_ = interception(interception_, ideal_, scale_, dimension_, elites);

    auto cost = std::unique_ptr<double[], decltype(&math::free)>{ math::allocate(dimension_), math::free };

    for (auto& elite : elites)
    {
        math::copy(dimension_, elite + scale_, 1, cost.get(), 1);
        attach(normalize(dimension_, cost.get(), ideal_, interception_));
    }

    while(!critical.empty())
    {
        auto individual = *critical.begin();
        math::copy(dimension_, individual + scale_, 1, cost.get(), 1);
        associate(individual, normalize(dimension_, cost.get(), ideal_, interception_));
        critical.pop_front();
    }

    auto compare = [](std::unique_ptr<Point>& lhs, std::unique_ptr<Point>& rhs)
    {
        bool left = lhs->associated.empty();
        bool right = rhs->associated.empty();
        return left ? false : (right ? true : lhs->count < rhs->count);
    };

    while (needed-- != 0)
    {
        points_.sort(compare);

        elites.push_back(*(*points_.begin())->associated.begin());
        (*points_.begin())->associated.pop_front();

        (*points_.begin())->count++;
    }

    for (auto& point : points_)
    {
        point->count = 0;
        critical.splice(critical.end(), point->associated);
    }
}

std::pair<std::list<double*>, std::list<double*>> UNSGA::Population::Reference::operator ()(std::list<std::list<double*>> layers)
{
    std::pair<std::list<double*>, std::list<double*>> result{ *layers.begin(), {} };
    layers.pop_front();

    // move the better individuals into the solution set
    while (result.first.size() + layers.begin()->size() < selection_)
    {
        result.first.splice(result.first.end(), *layers.begin());
        layers.pop_front();
    }

    // niche technology needed
    if (selection_ > result.first.size())
    {
        dispense(selection_ - result.first.size(), result.first, *layers.begin());
    }

    // move the left one to the population for cross and mutation operation
    for (auto& layer : layers) 
    {
        result.second.splice(result.second.end(), layer);
    }

    return result;
}