#include "unsga.h"

std::list<std::list<size_t>> Plain(size_t objectives, float division)
{
    if(objectives == 1)
    {
        return std::list<std::list<size_t>>{{fraction}};
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

UNSGA::Reference::Reference(size_t obejctives, size_t division)
{
//need to convert to the vector, and scaled by 1/ N
    std::list<std::list<size_t>> plain = Plain(objectives, division);

    for(const auto& point : plain)
    {
        points.push_back(std::make_unique<Point>(division, point));
    }
}

void Ideal(const std::list<Individual*>& individuals, size_t size, float * ideal)
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
    float * weights = mkl_malloc(size * sizeof(float), 64);
    std::fill(weigths, weights + size, 1e-6);
    weights[dimension] = 1;

    vsDiv(size, objective, weights, weights);
    float result = std::max_element(weights, weights + size);

    mkl_free(weights);
    return result;
}

void Interception(const std::list<Individual*>& solution, size_t objectives, const float * ideal, const float * cost, float * interception)
{
    std::vector<Individual*> extremes(objectives, nullptr);
    std::vector<float> minimums(objectives, +INFINITY);
    float * matrix = mkl_malloc(objectives * objectives * sizeof(float), 64);

	for (const auto& individual : solution)
	{
		for (size_t objective = 0; i < objectives; ++objective)
		{
            float asf = Scale(objectives, objective, cost);

			if (minimums[objective] > asf)
			{
                minimums[objective] = asf;
				extreme[objective] = *individual;
			}
		}
        cost += objectives;
	}

    std::fill(interception, interception + size, 1);
    for(const auto& individual : extremes)
    {
        vsSub(objectives, individual->objective(), ideal, matrix);
        matrix += objectives;
    }

	int ipiv[objectives], info;
	sgesv(&objectives, 1, matrix,  objectives, ipiv, interception,objectives, &info);

	float one = 1;
	vsDivI(Individual::size(), &one, 0, interception_, 1, interception, 1);
}

void Normalize(const std::list<Individual*>& solution, size_t size, float * cost)
{
    for(const auto& individual : solution)
    {
        vsSub(size, individual->objective(), ideal, cost);
        vsDiv(size, cost, interception, cost);
        cost += size;
    }
}

void UNSGA::Reference::Niche(std::list<Individual*>& solution, std::list<Individual*>& critical)
{
    float * ideal = mkl_malloc(size * sizeof(float), 64);
    float * interception = mkl_malloc(size * sizeof(float), 64);
    
    Ideal(solution, ideal);
    Interception();
    Normalize(solution, ideal, interception, cost);
}