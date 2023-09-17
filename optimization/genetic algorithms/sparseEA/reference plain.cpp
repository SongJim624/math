#include "sparseea.h"
/**************************************************************************
 *  non dominated sort
 ***************************************************************/
int dominate(size_t length, const double *lhs, const double *rhs)
{
    size_t counts[3] = { 0, 0, 0 };

    for(auto l = lhs, r = rhs; r != rhs + length; ++l, ++r)
    {
        counts[std::abs(*l - *r) < 1e-6 ? 1 : (*l > *r ? 0 : 2)]++;
    }

    return (counts[1] == length) ? 0 : ((counts[0] == 0) ? 1 : ((counts[2] == 0) ? -1 : 0));
}

int dominate(size_t scale, size_t dimension, size_t constraint, const Individual& lhs, const Individual& rhs)
{
//  compare the voilations of the constraints first
    int status = dominate(constraint, lhs.first + scale + dimension, rhs.first + scale + dimension);
//  compare the objectives if no constraints voilation of the two individuals
    return status != 0 ? status : dominate(dimension, lhs.first + scale, rhs.first + scale);
}

std::pair<bool, std::list<Individual>> sort(size_t scale, size_t dimension, size_t constraint, const Individual& individual, Series& current)
{
    auto results = std::make_pair<bool, std::list<Individual>>(true, {});
    auto& [status, dominated] = results;

    for(auto member = current.begin(); member != current.end(); void())
    {
        auto indicator = dominate(scale, dimension, constraint, *member, individual);

    //  if the individual is dominated by a member in the current layer
    //  individuals that the individual can dominate have been sorted into lower layers during previous sorting
    //  the lower set must be empty and hence no need for further comparison
        if(indicator == 1) {  status = false; break; }

        indicator == -1 ? dominated.push_back(*member) : void();
        member = indicator == -1 ? current.erase(member) : std::next(member);
    }

    return results;
}

std::list<Series> sort(size_t scale, size_t dimension, size_t constraint, const Series& individuals)
{
//  initialize the layer, move an individual from population to the layer
    auto individual = individuals.begin();
    std::list<Series> results = { { *individual } };

//  the non dominated sort is re-designed per bubble sort idea
    for(individual++; individual != individuals.end(); ++individual)
    {
    //  bubble sorting loop
        for (auto layer = results.begin(); layer != results.end(); ++layer)
        {
        //  status : dominating status, true if not dominated by any members in the current layer
        //  lower : individuals dominated by the individual and moved out from the current layer
            auto [status, lower] = sort(scale, dimension, constraint, *individual, *layer);

        //  if the individual is dominated by a member in the current layer
        //  and there are still further layers, continue the bubble sorting loop
            if(!status && (std::next(layer) != results.end())) { continue; }

        //  if the indiviudual is not dominated by any member in the current layer
        //  add it to the current layer, or do nothing
            status ? layer->push_back(*individual) : void();

        //  if the individual is dominated by a member in the current layer and there is no further layer
        //  create a layer at the end of results and store the individual in it
        //  if the individual is not dominated by any member in the current layer and dominateds some inviduals
        //  create a new lower layer to store the individuals, which still maintains the dominating relationship
            !status ? results.insert(results.end(), { *individual }) : lower.empty() ? layer : results.insert(std::next(layer), lower);

        //  exit the buble sorting loop
            break;
        }
    }

    return results;
}

std::list<Series> Reference::sort(Series&& population) const
{
//    size_t scale_, dimension_, constraint_;
    return ::sort(scale_, dimension_, constraint_, population);
}

/**************************************************************************
 *  elite reserve selection
 ***************************************************************/
using Association = std::map<double*, std::pair<size_t, std::list<Individual>>>;

void doolittle(size_t dimension, double* matrix, double* vector)
{
    auto temporary = create<double>(dimension * dimension);

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

//  the scalar function, asf function in the article
double scale(size_t position, size_t dimension, const double * objective)
{
    auto weights = create<double>(dimension);
    weights[position] = 1;

    math::div(dimension, objective, weights.get(), weights.get());
    return *std::max_element(weights.get(), weights.get() + dimension);
}

//  compute the perpendicular distance between the objective of an individual and the reference point
double distance(size_t length, const double * point, const double * objective)
{
    auto temporary = create<double>(length);
    math::copy(length, point, 1, temporary.get(), 1);

    double fraction = -math::dot(length, point, 1, objective, 1) / math::dot(length, point, 1, point, 1);
    math::xpby(length, objective, 1, fraction, temporary.get(), 1);
    return std::sqrt(math::dot(length, temporary.get(), 1, temporary.get(), 1));
}

double* ideal(double* point, size_t scale, size_t dimension, const Series& individuals)
{
    std::fill(point, point + dimension, double(+INFINITY));

    for (const auto& individual : individuals)
    {
        for (auto pos = point, loc = individual.first; pos != point + dimension; ++pos, ++loc)
        {
            *pos = std::min(*pos, *loc);
        }
    }

    return point;
}

double* interception(double* values, const double * ideal, size_t scale, size_t dimension, const Series& individuals)
{
    auto cost = std::unique_ptr<double[], decltype(&math::free<double>)>{ math::allocate<double>(dimension), math::free<double> };
    auto matrix = std::unique_ptr<double[], decltype(&math::free<double>)>{ math::allocate<double>(dimension * dimension), math::free<double> };
    
    std::vector<std::pair<double, double*>> nearest(dimension, { double(+INFINITY), nullptr});

    for (auto& individual : individuals)
    {
        math::sub(dimension, individual.first + scale, ideal, cost.get());

        for (size_t axis = 0;  axis < dimension; ++axis)
        {
            double distance = ::scale(axis, dimension, cost.get());

            if (distance < nearest[axis].first)
            {
                nearest[axis].first = distance;
                nearest[axis].second = individual.first;
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

//  attach the individual in the elite set to the reference plain
void attach(size_t dimension, Individual individual, const double *cost, Association &associations)
{
    std::pair<double, size_t*> nearest = { +INFINITY, nullptr };

    for(auto& [point, association] : associations)
    {
        double distance = ::distance(dimension, point, cost);

        if (distance < nearest.first)
        {
            nearest.first = distance;
            nearest.second = &association.first;
        }
    }

    (*nearest.second) += 1;
}

//  attach the individual in the critical set to the reference plain
void associate(size_t dimension, Individual individual, const double *cost, Association &associations)
{
    std::pair<double, std::list<Individual>*> nearest = { +INFINITY, nullptr };

    for(auto& [point, association] : associations)
    {
        double distance = ::distance(dimension, point, cost);

        if (distance < nearest.first)
        {
            nearest.first = distance;
            nearest.second = &association.second;
        }
    }

    nearest.second->push_back(individual);
}

void Reference::dispense(size_t needed, Series& elites, Series& criticals)
{
    ideal(ideal_.get(), scale_, dimension_, elites);
    interception(interception_.get(), ideal_.get(), scale_, dimension_, elites);

    auto cost = Pointer<double>{ math::allocate<double>(dimension_), math::free<double> };

    for (auto& elite : elites)
    {
        math::copy<double>(dimension_, elite.first + scale_, 1, cost.get(), 1);
        attach(dimension_, elite, normalize(dimension_, cost.get(), ideal_.get(), interception_.get()), associations_);
    }

    for(auto& individual : criticals)
    {
        math::copy(dimension_, individual.first + scale_, 1, cost.get(), 1);
        associate(dimension_, individual, normalize(dimension_, cost.get(), ideal_.get(), interception_.get()), associations_);
    }
    criticals.clear();

    auto sort = [](Association& associations)
        {
            auto min = associations.begin();

            for(auto iter = associations.begin(); iter != associations.end(); ++iter)
            {
                if(iter->second.second.empty()) { continue; }

                if(min->second.second.empty()) { min = iter; continue; }

                if(iter->second.first < min->second.first) { min = iter; continue; }
            }

            return min;
        };

    while (needed-- != 0)
    {
        auto& [count, associated] = sort(associations_)->second;

        elites.push_back(*associated.begin());
        associated.pop_front();

        count++;
    }

    for (auto& [point, association] : associations_)
    {
        association.first = 0;
        criticals.splice(criticals.end(), association.second);
    }
}

std::pair<Series, Series> Reference::select(Series&& population)
{
    auto layers = sort(std::forward<Series>(population));
    auto results = std::make_pair<>(std::move(*layers.begin()), Series());
    auto& [elite, ordinary] = results;

//  move the better individuals into the solution set
    while(elite.size() + layers.begin()->size() < selection_)
    {
        elite.splice(elite.end(), *layers.begin());
        layers.pop_front();
    }

//  size judge contained in the dispense function
    if(selection_ > elite.size() )
    {
        dispense(selection_ - elite.size(), elite, *layers.begin());
    }

//  move the left one to the population for cross and mutation operation
    for (auto& layer : layers)
    {
        ordinary.splice(ordinary.end(), layer);
    }

    return results;
}
/**************************************************************************
 *  reference plaint constructor
 ***************************************************************/

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

Reference::Reference(math::Optimizor::Configuration& configuration) :
    scale_(std::get<size_t>(configuration["scale"])),
    dimension_(std::get<size_t>(configuration["dimension"])),
    selection_(std::get<size_t>(configuration["population"]) / 2),
    ideal_(create<double>(dimension_)), interception_(create<double>(dimension_))
{
    size_t division = std::get<size_t>(configuration["division"]);

    for (const auto& point : permutation(dimension_, division))
    {
        points_.push_back(create<double>(dimension_));
        auto pointer = points_.rbegin()->get();

        std::copy(point.begin(), point.end(), pointer);
        math::scal(dimension_, 1.0 / division, pointer, 1);

        associations_.insert(associations_.end(), { pointer, { 0, {} }});
    }
}