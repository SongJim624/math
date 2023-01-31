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

            for(size_t point = 0; i < amount; point++)
            {
                (*point)[0] = i;
                selected[i] = (*point) + 1;
                point++;
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

UNSGA::Reference::Reference(size_t objectives, size_t division)
{
    std::list<std::list<size_t>> plain = Plain(objectives, division);

    for(const auto& point : plain)
    {
        points.push_back(std::make_unique<Point>(division, point));
    }
}

void UNSGA::Reference::Ideal(const std::list<Individual*>& individuals, float* ideal)
{
    for(const auto& individual : individuals)
    {
        for(size_t i = 0; i < size, ++i)
        {
            ideal[i] = fminf(ideal[i], individual->objective()[i]);
        }
    }
}

//achievement scalar function
float Scale(size_t size, size_t dimension, const float * objective)
{
    float * weights = (float *) mkl_malloc(size * sizeof(float), 64);
    std::fill(weights, weights + size, 1e-6);
    weights[dimension] = 1;

    vsDiv(size, objective, weights, weights);
    float* result = std::max_element(weights, weights + size);

    mkl_free(weights);
    return *result;
}

void UNSGA::Reference::Interception(const std::list<Individual*>& solution, const float * ideal, float * interception)
{
    std::vector<Individual*> extremes(objectives_, nullptr);
    std::vector<float> minimums(objectives_, +INFINITY);
    float * matrix = (float *) mkl_malloc(objectives_ * objectives * sizeof(float), 64);

	for (const auto& individual : solution)
	{
		for (size_t objective = 0; objective < objectives_; ++objective)
		{
            vsSub(dimension_, individual->objectives, ideal, matrix)
            float asf = Scale(objectives_, objective, matrix);

			if (minimums[objective] > asf)
			{
                minimums[objective] = asf;
				extremes[objective] = *individual;
			}
		}
        cost += objectives_;
	}

    std::fill(interception, interception + objectives_, 1);
    for(const auto& individual : extremes)
    {
        vsSub(objectives, cost, ideal, matrix);
        matrix += objectives;
    }

	int ipiv[objectives], info;
	sgesv(&objectives, 1, matrix,  objectives, ipiv, interception,objectives, &info);

	float one = 1;
	vsDivI(Individual::size(), &one, 0, interception_, 1, interception, 1);
}

UNSGA::Reference::Cost UNSGA::Reference::Normalize(const std::list<Individual*> solution, const std::list<Individual*>& critical, float* costs)
{
    float * ideal = Ideal(solution);
    float * interception = Ideal(solution, ideal);

    Cost result({{}, {}});

    for(const auto& individual : solution)
    {
        vsSub(objectives_, individual->objectives, ideal, costs);
        vsDiv(objectives_, costs, interception, costs);
        result.first.insert({ individual, costs });

        costs += size;
    }

    for (const auto& individual : critical)
    {
        vsSub(size, individual->objectives(), ideal, costs);
        vsDiv(size, costs, interception, costs);
        result.second.insert({ individual, costs });

        costs += size;
    }

    mkl_free(ideal);
    mkl_free(interception);

    ideal = nullptr;
    interception = nullptr;

    return result;
}


void UNSGA::Reference::Associate(const std::pair<std::map<Individual*, float*>, std::map<Individual*, float*>>& costs)
{
    for (const auto& [individual, cost] : costs.first)
    {
        std::pair<float, Point*> nearest(0, nullptr);

        for (auto& point : points_)
        {
            float distance = point->distance(cost);

            if (distance < nearest.first)
            {
                nearest.first = distance;
                nearest.second = point;
            }
        }

        nearest.second->count++;
    }

    std::map<Point*, std::map<float, Individual*>> association;

    for (const auto& [individual, cost] : costs.second)
    {
        std::pair<float, Point*> nearest(0, nullptr);

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
    /*
    * The choice of Zr should be random when the number of the points whose rho equals 0 is larger than one
    * Here the first one of the list is used directly
    * because the rho would be one after the individual associated with the point pushed into solution
    * the later whose rho is also 0 would become the begin of the list
    */
    for (size_t i = 0; i < needed; ++i)
    {
        points_.sort([](Point* lhs, Point* rhs) {
            bool left = lhs->associated.empty();
        bool right = rhs->associated.empty();

        return left ? false : (right ? true : lhs->count < rhs->count);
            });

        //         the associated points should be sorted after attached.
       //        auto point = points_.begin();
       //        point->individuals.sort(DistanceCMP());

        solution.splice(solution.end(), (*points_.begin())->associated, (*points_.begin())->associated.begin());
        (*points_.begin())->count++;
    }
}

void UNSGA::Reference::Niche(size_t needed, std::list<UNSGA::Individual*>& solution, std::list<Individual*>& ciritical)
{
    float  * costs = (float *) mkl_malloc((solution.size() + critical.size()) *  objectives_ * sizeof(float), 64);

    Associate(Normalize(solution, critical, costs));
    Despense(needed, solution, critical);

    mkl_free(costs);
    costs = nullptr;
}
