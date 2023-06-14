#include "unsga.h"

size_t Combination(size_t dimension, size_t division)
{
    dimension += division - 1;
    division = std::min(division, dimension - division);
    size_t result = 1;

    for(size_t i = 0; i < division; ++i)
    {
        result *= dimension - i;
    }

    for(size_t i = 0; i < division; ++i)
    {
        result /= division - i;
    }

    return result;
}

void Plain(size_t dimension, size_t division, std::vector<float*>& points)
{
    if(dimension == 1)
    {
        points[0][0] = division;
    }
    else
    {
        for(size_t i = 0; i < division + 1; ++i)
        {
            size_t amount = Combination(dimension - 1, division - i);
            std::vector<float*> selected(amount);

            auto point = points.begin();
            for(size_t j = 0; j < amount; ++j)
            {
                (*point)[0] = i;
                selected[j] = (*point) + 1;
                point++;
            }

            Plain(dimension - 1, division - i, selected);
        }
    }
}

UNSGA::Reference::Reference(std::shared_ptr<Configuration> configuration)
    : configuration_(configuration)
{
    size_t dimension = configuration_->dimension;
    size_t amount = Combination(configuration_->dimension, configuration_->division);

    storage_.resize(amount);
    std::vector<float*> locations(amount, (float*) mkl_malloc(dimension * sizeof(float), 64));

    Plain(configuration_->dimension, configuration_->division, locations);

    for (size_t i = 0; i < amount; ++i)
    {
        cblas_sscal(configuration_->dimension, 1.0 / configuration_->division, locations[i], 1);
        storage_[i] = std::make_unique<Point>(locations[i], configuration_);

        points_.push_back(storage_[i].get());
        mkl_free(locations[i]);
    }
}

UNSGA::Reference::~Reference()
{
}

UNSGA::vector UNSGA::Reference::Ideal(const std::list<Individual*>& individuals)
{
    UNSGA::vector ideal(configuration_->dimension);
    for(const auto& individual : individuals)
    {
        for(size_t i = 0; i < configuration_->dimension; ++i)
        {
            ideal[i] = fminf(ideal[i], individual->objectives[i]);
        }
    }

    return ideal;
}

//achievement scalar function
float Scale(size_t size, size_t dimension, const UNSGA::vector& objective)
{
    std::vector<float, UNSGA::Allocator<float>> weights(size, 1e-6);
    weights[dimension] = 1;

    vsDiv(size, &objective[0], &weights[0], &weights[0]);
    return *std::max_element(weights.begin(), weights.end());
}

UNSGA::vector UNSGA::Reference::Interception(const std::list<Individual*>& solution, const UNSGA::vector& ideal)
{
    int dimension = configuration_->dimension;

    std::vector<float, UNSGA::Allocator<float>> interception(dimension, 1);
    std::vector<float, UNSGA::Allocator<float>> matrix(dimension * dimension);

    std::vector<std::map<float, Individual*>> rank(dimension);

	for (auto individual : solution)
	{
		for (size_t objective = 0; objective < dimension; ++objective)
		{
            vsSub(dimension, &(individual->objectives[0]), &ideal[0], &matrix[0]);
            rank[objective].insert({ Scale(dimension, objective, matrix), individual });
		}
	}

    for(size_t i = 0; i < dimension; ++i)
    {
        vsSub(dimension, &(rank[i].begin()->second->objectives[0]), &ideal[0], &matrix[i * dimension]);
    }

    std::vector<int>ipiv(dimension);
    int info = 0, column = 1;
	sgesv(&dimension, &column, &matrix[0], &dimension, &ipiv[0], &interception[0], &dimension, &info);

	float one = 1;
	vsDivI(configuration_->dimension, &one, 0, &interception[0], 1, &interception[0], 1);

    return interception;
}

UNSGA::Reference::Cost UNSGA::Reference::Normalize(const std::list<Individual*> solution, const std::list<Individual*>& critical)
{
    size_t dimension = configuration_->dimension;

    auto ideal = Ideal(solution);
    auto interception = Interception(solution, ideal);

    Cost result({{}, {}});

    for(const auto& individual : solution)
    {
        std::vector<float, UNSGA::Allocator<float>> cost(dimension);

        vsSub(dimension, &(individual->objectives[0]), &ideal[0], &cost[0]);
        vsDiv(dimension, &cost[0], &interception[0], &cost[0]);
        result.first.insert({ individual, cost });
    }

    for (const auto& individual : critical)
    {
        std::vector<float, UNSGA::Allocator<float>> cost(dimension);

        vsSub(dimension, &(individual->objectives[0]), &ideal[0], &cost[0]);
        vsDiv(dimension, &cost[0], &interception[0], &cost[0]);
        result.second.insert({ individual, cost });
    }

    return result;
}

void UNSGA::Reference::Associate(const Cost& costs)
{
    for (const auto& [individual, cost] : costs.first)
    {
        std::map<float, Point*> rank;

        for (auto& point : points_)
        {
            rank.insert({ point->distance(cost), point });
        }

        rank.begin()->second->count++;
    }

    std::map<Point*, std::map<float, Individual*>> association;

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

void UNSGA::Reference::Dispense(size_t needed, std::list<UNSGA::Individual*>& solution, std::list<Individual*>& critical)
{
    for (size_t i = 0; i < needed; ++i)
    {
        points_.sort([](Point* lhs, Point* rhs) {
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

void UNSGA::Reference::Niche(size_t needed, std::list<UNSGA::Individual*>& solution, std::list<Individual*>& critical)
{
    Associate(Normalize(solution, critical));
    Dispense(needed, solution, critical);
}

std::pair<std::list<UNSGA::Individual*>, std::list<UNSGA::Individual*>> UNSGA::Reference::Select(std::list<std::list<Individual*>>& layers)
{
    std::pair<std::list<Individual*>, std::list<Individual*>> result{ {}, {} };

    size_t selection = 0;
    for(const auto& layer : layers)
    {
        selection += layer.size();
    }
    selection /= 2;

    auto& elites = result.first;
    auto& ordinary = result.second;

    //move the better individuals into the solution set
    while (true)
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
    if (selection > elites.size())
    {
        Niche(selection - elites.size(), elites, *layers.begin());
    }

    //nove the left one to the population for cross and mutation operation
    while (layers.size() != 0)
    {
        ordinary.splice(ordinary.end(), *layers.begin());
        layers.pop_front();
    }

    return result;
}

size_t UNSGA::Reference::size() const
{
    return points_.size();
}