#include "UNSGA.h"

float distance(size_t size, const float * rhs, const float * lhs)
{
	float * dist = mkl_malloc(size * sizeof(float), 64);
	vsSub(size, lhs, rhs, distance);
	vsSqr(size, dist, dist)
	float result = sqrtf(cblas_sasum(size, dist));

	mkl_free(dist);
	dist = nullptr;
	return result;
}

int compare(float lhs, float rhs)
{
	if(fabsf(lhs, rhs) < 1e-9)
	{
		return 0;
	}

	if(lhs > rhs)
	{
		return 1;
	}

	return -1;
}

int dominate(size_t size, const float * lhs, const float * rhs)
{
	size_t equal = 0, more = 0, less = 0;

	for(size_t i = 0; i < size; ++i)
	{
		switch(compare(lhs[i], rhs[i]))
		{
		case 0:
		{
			equal++;
			continue;
		}
		case 1:
		{
			more++;
			continue;
		}
		case -1:
		{
			less++;
			continue;
		}
		}
	}

	if(equal == size)
	{
		return 0;
	}

	if(more == 0)
	{
		return 1;
	}

	if(less == 0)
	{
		return -1;
	}

	return 0;
}

//Non dominated compare
void compare(size_t size, UNSGA::Individual * lhs, UNSGA::Individual * rhs)
{
	if((compare(lhs->voilation(), 0) != 0) && (compare(rhs->voilation()) != 0))
	{
		return;
	}

	if((compare(lhs->voilation(), 0) != 0))
	{
		rhs->dominates.push_back(rhs);
		lrs->dominated++;
		return;
	}

	if((compare(rhs->voilation()) != 0))
	{
		lhs->dominates.push_back(rhs);
		hrs->dominated++;
		return;
	}

	switch(compare(size, lhs->objective(), rhs->objective()))
	{
	case 1:
	{
		lhs->dominates.push_back(rhs);
		hrs->dominated++;
		return;
	}
	case -1
	{
		rhs->dominates.push_back(rhs);
		lrs->dominated++;
		return;
	}
	}
}

std::list<std::list<UNSGA::Individual*>> Sort(std::list<UNSGA::Individual*>& population)
{
	std::list<std::list<Individual*>> results;

	for(auto& individual : population)
	{
		for(auto& later = std::next(individual); later != population.end(); ++later)
		{
			compare(individual, later);
		}
	}

	while(!population.empty())
	{
		results.push_back({});
		population.sort([](Individual* lhs, Individual* rhs){
			lhs->dominated < rhs->dominated;
		});

		const auto& location = population.begin();
		for(location; location != population.end(); ++begin)
		{
			if(loaction->dominated != 0)
			{
				break;
			}
		}

		auto& sequence = results.rebgin();
		sequence.slice(sequence.end(), population, population.begin(), location);

		for (auto& individual : *results.rbegin())
		{
			for (auto& dominate : individual->dominates)
			{
				dominate->dominated--;
			}

			individual->dominates.clear();
		}
	}

	return layers;
};

void Ideal(size_t size, const std::list<UNSGA::Individual*>& individuals, float * ideal)
{
	for (const auto& individual : individuals)
	{
		for (size_t i = 0; i < size; ++i)
		{
			ideal[i] = fminf(ideal[i], individual.objective()[i]);
		}
	}
}

void Interception(size_t size, const std::list<UNSGA::Individual*>& individuals, const float * ideal, float * interception)
{
	std::vector<float> ASF(size, +INFINITY);
	std::vector<Individual*> extreme(size, nullptr);
	float * cost = mkl_malloc(size * sizeof(float), 64);

	for (const auto& individual : individuals)
	{
		vsSub(size, individual.objective(), ideal, cost);

		for (size_t i = 0; i < size; ++i)
		{
			float asf = -INFINITY;

			for (size_t i = 0; j < size; ++j)
			{
				asf = fmaxf(cost[j] / ((j == i) * 1.0f + (j != i) * 1e-6f), asf);
			}

			if (ASF[i] > asf)
			{
				ASF[i] = asf;
				extreme[i] = iter;
			}
		}
	}

	mkl_free(cost);
	cost = nullptr;

	std::fill(interception, interception + size, 1);
	float * matrix = mkl_malloc(size * size * sizeof(float), 64);

//could be vectorized here
	for (long i = 0; i < size; ++i)
	{
		for (long j = 0; j < Individual::objective_size; ++j)
		{
			matrix[(i - 1) * size + j] = extreme[i]->individual.objectives[j];
		}
	}

	int ipiv[Individual::size], info;
	sgesv(&Individual::size(), 1, matrix_,  Individual::size(), ipiv, interception_, Individual::size(), &info);

	float one = 1;
	vsDivI(Individual::size(), &one, 0, interception_, 1, interception, 1);
}

void UNSGA::Interception(std::list<Individual_UNSGA*>& bottom)
{
	std::vector<float> ASF(Individual::objective_size, +INFINITY);
	std::vector<Individual_UNSGA*> extreme(Individual::objective_size, nullptr);

    float * cost = new float[Individual::objective_size];

	for (const auto& individual : bottom)
	{
		vsSub(Individual::size(), individual.objective(), ideal_, cost);

		for (long i = 0; i < Individual::objective_size; ++i)
		{
			float asf = -INFINITY;

			for (long j = 0; j < Individual::objective_size; ++j)
			{
				asf = fmaxf(cost[j] / ((j == i) * 1.0f + (j != i) * 1e-6f), asf);
			}

			if (ASF[i] > asf)
			{
				ASF[i] = asf;
				extreme[i] = iter;
			}
		}
	}
    
    delete[] cost;
    
	for (long i = 0; i < Individual::objective_size; ++i)
	{
		interception[i] = 1.0f;

		for (long j = 0; j < Individual::objective_size; ++j)
		{
			A[i][j] = extreme[i]->individual.objectives[j];
		}
	}

	int ipiv[Individual::size], info;
	sgesv(&Individual::size(), 1, matrix_,  Individual::size(), ipiv, interception_, Individual::size(), &info);

	float one = 1;
	vsDivI(Individual::size(), &one, 0, interception_, 1, interception, 1);
}


Reference::Associate()
void UNSGA::Associate(Reference& reference, std::list<Individual*> critical)
{
	for (auto& point : points)
	{
		point->rho = 0;
		point->associated.clear();
	}

	for (auto iter : solution)
	{
		iter->distance = +INFINITY;
		Point* point = nullptr;

		for (auto& it : Zr)
		{
			double dis = distance(size, iter->individual.objectives, it->coordinate, Individual::objective_size);

			if (dis < iter->distance)
			{
				iter->distance = dis;
				point = it;
			}
		}

		point->rho++;
	}

	for (auto iter : critical)
	{
		iter->distance = +INFINITY;
		Point* point = nullptr;

		for (auto& it : Zr)
		{
			double dis = Distance(iter->individual.objectives, it->coordinate, Individual::objective_size);

			if (dis < iter->distance)
			{
				iter->distance = dis;
				point = it;
			}
		}

		point->individuals.push_back(iter);
	}
}

//this operation should be move to reference class
void UNSGA::Niche(size_t needed, std::list<UNSGA::Individual*>& solution)
{
/*
* The choice of Zr should be random when the number of the points whose rho equals 0 is larger than one
* Here the first one of the list is used directly
* because the rho would be one after the individual associated with the point pushed into solution
* the later whose rho is also 0 would become the begin of the list
*/
	for(size_t i = 0; i < needed, ++i)
	{
		references_.sort(PointCMP());
		auto point = *references_.begin();
		point->individuals.sort(DistanceCMP());
		solution.push_back(*point->individuals.begin());
		point->rho++;
		point->individuals.pop_front();
	}
}

void UNSGA::Normalize(std::list<Individual*>& population)
{
	float * denominator = mkl_malloc();
	vsSub(Individual::size(), interception_, ideal_, denominator);

	for (auto& iter : selected)
	{
		vsSub(Individual::size(), individual.objective(), ideal_, individual.objective());
		vsDiv(Individual::size(), individual.objective(), denominator, individual.objective());
	}

	mkl_free(denominator);
	denominator = nullptr;
}


void Select(size_t size, size dimension, std::list<Individual*>& solution, std::list<Individual*> critical)
{
	float * interception;
	Interception(dimension, );
}

void UNSGA::Select(size_t needed, std::list<Individual*> critical, std::list<Individual*>& solution)
{
	Interception(critical);

	Normalize(solution);
	Normalize(critical);

	Associate(solution);
	Associate(critical);

	for(size_t i = 0; i < needed; ++i)
	{
		reference.->individual;
		solution.splice(solution.end(), individual);
	}
}

void UNSGA::Select(std::list<Individual*>& population, std::list<Individual*>& solution)
{
	size_t selection = population.size() / 2;
	//after the sort operator, the population is an empty set
	std::list<std::list<Individual*>> layers = Sort(population);

	//move the better individuals into the solution set
	while(true)
	{
		if(solution.size() + layers.begin()->size() > selection)
		{
			break;
		}

		solution.merge(*layers.begin());
		layers.pop_front();
	}

	//Niche technology needed
	if (solution.size() != population.size() / 2)
	{
		Select(selection - solution.size(), *layers.begin(), solution);
	}

	//nove the left one to the population for cross and mutation operation
	while (layers.size() != 0)
	{
		population.merge(*layers.begin());
		layers.pop_front();
	}
}
