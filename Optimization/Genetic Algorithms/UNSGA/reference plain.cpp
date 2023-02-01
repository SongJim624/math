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

void Plain(size_t dimension, size_t division, float ** points)
{
    if(dimension == 1)
    {
        **points = division;
    }
    else
    {
        for(size_t i = 0; i < division + 1; ++i)
        {
            size_t amount = Combination(dimension - 1, division - i);
            float ** selected = new float *[amount];

            for(size_t j = 0; j < amount; ++j)
            {
                (*points)[0] = i;
                selected[j] = (*points) + 1;
                points++;
            }

            Plain(dimension - 1, division - i, selected);

            delete[] selected;
            selected = nullptr;
        }
    }
}

std::list<std::list<size_t>> Plain(size_t objectives, size_t division)
{
    if(objectives == 1)
    {
        return std::list<std::list<size_t>>{{division}};
    }
    else
    {
        std::list<std::list<size_t>> points;

        for(size_t i = 0; i <= division; ++i)
        {
            auto plain = Plain(objectives - 1, division - i);

            for(auto& point : plain)
            {
                points.push_back({i});
                points.rbegin()->splice(points.rbegin()->end(), point);
            }
        }

        return points;
    }
}

UNSGA::Reference::Reference(std::shared_ptr<Configuration> configuration)
    : configuration_(configuration)
{
    size_t amount = Combination(configuration_->dimension, configuration_->division);
    locations_ = (float*)mkl_malloc(amount * configuration_->dimension * sizeof(float), 64);
    
    std::vector<float*> locations(amount, nullptr);

    for (size_t i = 0; i < amount; ++i)
    {
        locations[i] = locations_ + i * configuration_->dimension;
    }

    Plain(configuration_->dimension, configuration_->division, &locations[0]);
    cblas_sscal(amount * configuration_->dimension, 1.0 / configuration_->division, locations_, 1);
   
    for (size_t i = 0; i < amount; ++i)
    {
        points_.push_back(new Point(locations[i], configuration_));
    }
}

UNSGA::Reference::~Reference()
{
    for (auto& point : points_)
    {
        delete point;
        point = nullptr;
    }

    mkl_free(locations_);
    locations_ = nullptr;
}

void UNSGA::Reference::Ideal(const std::list<Individual*>& individuals, float* ideal)
{
    for(const auto& individual : individuals)
    {
        for(size_t i = 0; i < configuration_->dimension; ++i)
        {
            ideal[i] = fminf(ideal[i], individual->objectives[i]);
        }
    }
}

//achievement scalar function
float Scale(size_t size, size_t dimension, const float * objective)
{
    std::vector<float> weights(size, 1e-6);
    
    std::fill(weights.begin(), weights.end(), 1e-6);
    weights[dimension] = 1;

    vsDiv(size, objective, &weights[0], &weights[0]);
    return *std::max_element(weights.begin(), weights.end());
}

void UNSGA::Reference::Interception(const std::list<Individual*>& solution, const float * ideal, float * interception)
{
    int dimension = configuration_->dimension;

    std::vector<Individual*> extremes(dimension, nullptr);
    std::vector<float> minimums(dimension, +INFINITY);
    std::vector<float> matrix(dimension * dimension);

	for (auto individual : solution)
	{
		for (size_t objective = 0; objective < dimension; ++objective)
		{
            vsSub(dimension, &individual->objectives[0], ideal, &matrix[0]);
            float asf = Scale(dimension, objective, &matrix[0]);

			if (minimums[objective] > asf)
			{
                minimums[objective] = asf;
				extremes[objective] = individual;
			}
		}
	}

    std::fill(interception, interception + dimension, 1);
    
    auto row = matrix.begin();
    for(const auto& individual : extremes)
    {
        vsSub(dimension, &individual->objectives[0], ideal, &(*row));
        row += dimension;
    }

    int* ipiv = new int[dimension], info = 0;
    int column = 1;
	sgesv(&dimension, &column, &matrix[0], &dimension, ipiv, interception, &dimension, &info);
    delete[] ipiv;
    ipiv = nullptr;

	float one = 1;
	vsDivI(configuration_->dimension, &one, 0, interception, 1, interception, 1);
}

UNSGA::Reference::Cost UNSGA::Reference::Normalize(const std::list<Individual*> solution, const std::list<Individual*>& critical, float* costs)
{
    size_t dimension = configuration_->dimension;
    float* ideal = (float*)mkl_malloc(dimension * sizeof(float), 64);
    float* interception = (float*)mkl_malloc(dimension * sizeof(float), 64);

    Ideal(solution, ideal);
    Interception(solution, ideal, interception);

    Cost result({{}, {}});

    for(const auto& individual : solution)
    {
        vsSub(dimension, &individual->objectives[0], ideal, costs);
        vsDiv(dimension, costs, interception, costs);
        result.first.insert({ individual, costs });

        costs += dimension;
    }

    for (const auto& individual : critical)
    {
        vsSub(dimension, &individual->objectives[0], ideal, costs);
        vsDiv(dimension, costs, interception, costs);
        result.second.insert({ individual, costs });

        costs += dimension;
    }

    mkl_free(ideal);
    mkl_free(interception);

    ideal = nullptr;
    interception = nullptr;

    return result;
}


void UNSGA::Reference::Associate(const Cost& costs)
{
    for (const auto& [individual, cost] : costs.first)
    {
        std::map<float, Point*> rank;

//        std::pair<float, Point*> nearest(+INFINITY, nullptr);

        for (auto& point : points_)
        {
            rank.insert({ point->distance(cost), point });
//            float distance = point->distance(cost);

//            if (distance < nearest.first)
//           {
//                nearest.first = distance;
//                nearest.second = point;
//            }
        }
           
        rank.begin()->second->count++;
//        nearest.second->count++;
    }

    std::map<Point*, std::map<float, Individual*>> association;

    for (const auto& [individual, cost] : costs.second)
    {
        std::pair<float, Point*> nearest(+INFINITY, nullptr);

        for (auto& point : points_)
        {
            float distance = point->distance(cost);

            if (distance < nearest.first)
            {
                nearest.first = distance;
                nearest.second = point;
            }
        }

        association[nearest.second].insert({ nearest.first, individual });
    }

    for (const auto& [point, associated] : association)
    {
        for (const auto& [distance, individual] : associated)
        {
            point->associated.push_back(individual);
        }
    }
}

void UNSGA::Reference::Dispense(size_t needed, std::list<UNSGA::Individual*>& solution, std::list<Individual*>& ciritical)
{
    //
   // The choice of Zr should be random when the number of the points whose rho equals 0 is larger than one
   // Here the first one of the list is used directly
   // because the rho would be one after the individual associated with the point pushed into solution
   // the later whose rho is also 0 would become the begin of the list

    for (size_t i = 0; i < needed; ++i)
    {
        points_.sort([](Point* lhs, Point* rhs) {
            bool left = lhs->associated.empty();
            bool right = rhs->associated.empty();
            return left ? false : (right ? true : lhs->count < rhs->count);});

        solution.splice(solution.end(), (*points_.begin())->associated, (*points_.begin())->associated.begin());
        (*points_.begin())->count++;
    }
}

void UNSGA::Reference::Niche(size_t needed, std::list<UNSGA::Individual*>& solution, std::list<Individual*>& critical)
{
    float  * costs = (float *) mkl_malloc((solution.size() + critical.size()) *  configuration_->dimension * sizeof(float), 64);

    Associate(Normalize(solution, critical, costs));
    Dispense(needed, solution, critical);

    mkl_free(costs);
    costs = nullptr;
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

    //Niche technology needed
    Niche(selection - elites.size(), elites, *layers.begin());

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