#include "unsga.h"

UNSGA::Reproducor::Reproducor(std::shared_ptr<Configuration> configuration)
{
}

UNSGA::Reproducor::~Reproducor()
{
}
//Simulated Binary Cross 
void UNSGA::Reproducor::Cross(const Individual& father, const Individual& mother, Individual& son, Individual& daughter)
{
	size_t scale = configuration_->scale;

	float* random = (float*)mkl_malloc(scale * sizeof(float), 64);
	float* weights = (float*)mkl_malloc(scale * sizeof(float), 64);

	vsRngUniform(VSL_RNG_METHOD_UNIFORM_STD, configuration_->stream, scale, random, 0, 1);

	for (long i = 0; i < scale; ++i)
	{
		random[i] = (random[i] < 0.5) ? 2.0 * random[i] : 0.5 / (1.0 - random[i]);
	}

	float coefficient = 1 / (configuration_->cross + 1);
	vsPowI(scale, random, 1, &coefficient, 0, random, 1);

	coefficient = 1;
	vsAddI(scale, &coefficient, 0, random, 1, weights, 1);
	vsMul(scale, weights, father.decisions, son.decisions);
	vsSubI(scale, &coefficient, 0, random, 1, weights, 1);
	vsMul(scale, weights, mother.decisions, weights);
	vsAdd(scale, weights, son.decisions, son.decisions);

	vsSubI(scale, &coefficient, 0, random, 1, weights, 1);
	vsMul(scale, weights, father.decisions, daughter.decisions);
	vsAddI(scale, &coefficient, 0, random, 1, weights, 1);
	vsMul(scale, weights, mother.decisions, weights);
	vsAdd(scale, weights, daughter.decisions, son.decisions);

	cblas_sscal(scale, 0.5, son.decisions, 1);
	cblas_sscal(scale, 0.5, daughter.decisions, 1);

	mkl_free(random);
	mkl_free(weights);
	random = nullptr;
	weights = nullptr;
}

//Polynomial Mutation
void UNSGA::Reproducor::Mutate(Individual& individual)
{
	size_t scale = configuration_->scale;

//	size_t decisions = information_->decision();

	float* random = (float *) mkl_malloc(scale * sizeof(float), 64);
	bool* labels = (bool*)mkl_malloc(scale * sizeof(bool), 64);

	std::fill(labels, labels + configuration_->scale, false);
	vsRngUniform(VSL_RNG_METHOD_UNIFORM_STD, configuration_->stream, scale, random, 0, 1);

	for (size_t i = 0; i < scale; ++i)
	{
		if (random[i] > 0.5)
		{
			random[i] = 1 - random[i];
			labels[i] = true;
		}
	}

	cblas_sscal(scale, 2, random, 1);
	float coefficeint = 1 / (configuration_->mutation + 1);
	vsPowI(scale, random, 1, &coefficeint, 0, random, 1);
	coefficeint = 1;
	vsSubI(scale, random, 1, &coefficeint, 0, random, 1);

	for (size_t i = 0; i < scale; ++i)
	{
		if (labels[i])
		{
			random[i] = -random[i];
		}
	}

	vsSub(scale, configuration_->objective->upper(), configuration_->objective->lower(), individual.decisions);
	vsMul(scale, random, individual.decisions, random);
	vsAdd(scale, random, configuration_->objective->lower(), individual.decisions);

	mkl_free(random);
	mkl_free(labels);
	random = nullptr;
	labels = nullptr;
}

//this function is to correct the values if the integer constraints applied
//needed not worry about the exceed of the upper and lower limite
//because the cross over and mutation has limited the values with the range
void UNSGA::Reproducor::check(Individual& individual)
{
	for (size_t i = 0; i < configuration_->dimension; ++i)
	{
		individual.decisions[i] = std::max(std::min(individual.decisions[i], configuration_->objective->upper()[i]), configuration_->objective->lower()[i]);

		if (configuration_->objective->integer()[i])
		{
			individual.decisions[i] = std::round(individual.decisions[i]);
		}
	}
}

std::list<UNSGA::Individual*> UNSGA::Reproducor::Reproduce(std::pair<std::list<Individual*>, std::list<Individual*>>& population)
{
	std::list<Individual*> result, temporary;

	auto& elites = population.first;
	auto& ordinary = population.second;

	if (elites.size() % 2)
	{
		ordinary.push_front(*elites.rbegin());
		elites.pop_back();
	}

	float* random = (float*)mkl_malloc(2 * sizeof(float), 64);

	auto father = elites.begin();
	auto mother = std::next(father);

	while (true)
	{
		if (ordinary.empty())
		{
			break;
		}

		auto son = ordinary.rbegin();
		auto daughter = std::next(son);

		Cross(**father, **mother, **son, **daughter);

		vsRngUniform(VSL_RNG_METHOD_UNIFORM_STD, configuration_->stream, 2, random, 0, 1);

		if (random[0] > configuration_->threshold)
		{
			Mutate(**son);
		}

		if (random[1] > configuration_->threshold)
		{
			Mutate(**daughter);
		}

		temporary.push_back(*son);
		temporary.push_back(*daughter);

		ordinary.pop_back();
		ordinary.pop_back();

		std::advance(father, 2);
		if (father == elites.end())
		{
			break;
		}

		std::advance(mother, 2);
	}

	result.splice(result.end(), elites);
	result.splice(result.end(), temporary);
	result.splice(result.end(), ordinary);
	return result;
}