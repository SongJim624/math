#include "unsga.h"

UNSGA::Reproducor::Reproducor(std::shared_ptr<Configuration> configuration)
	: configuration_(configuration)
{
}

UNSGA::Reproducor::~Reproducor()
{
}
//Simulated Binary Cross 
void UNSGA::Reproducor::Cross(const Individual& father, const Individual& mother, Individual& son, Individual& daughter)
{
	size_t scale = configuration_->scale;

	std::vector<float, UNSGA::Allocator<float>> random(scale);
	std::vector<float> weights(scale);

	vsRngUniform(VSL_RNG_METHOD_UNIFORM_STD, configuration_->stream, scale, &random[0], 0, 1);

	for (long i = 0; i < scale; ++i)
	{
		random[i] = (random[i] < 0.5) ? 2.0 * random[i] : 0.5 / (1.0 - random[i]);
	}

	float coefficient = 1 / (configuration_->cross + 1);
	vsPowI(scale, &random[0], 1, &coefficient, 0, &random[0], 1);

	coefficient = 1;
	vsAddI(scale, &coefficient, 0, &random[0], 1, &weights[0], 1);
	vsMul(scale, &weights[0], &father.decisions[0], &son.decisions[0]);
	vsSubI(scale, &coefficient, 0, &random[0], 1, &weights[0], 1);
	vsMul(scale, &weights[0], &mother.decisions[0], &weights[0]);
	vsAdd(scale, &weights[0], &son.decisions[0], &son.decisions[0]);

	vsSubI(scale, &coefficient, 0, &random[0], 1, &weights[0], 1);
	vsMul(scale, &weights[0], &father.decisions[0], &daughter.decisions[0]);
	vsAddI(scale, &coefficient, 0, &random[0], 1, &weights[0], 1);
	vsMul(scale, &weights[0], &mother.decisions[0], &weights[0]);
	vsAdd(scale, &weights[0], &daughter.decisions[0], &son.decisions[0]);

	cblas_sscal(scale, 0.5, &son.decisions[0], 1);
	cblas_sscal(scale, 0.5, &daughter.decisions[0], 1);

//	mkl_free(random);
//	mkl_free(weights);
//	random = nullptr;
//	weights = nullptr;
}

//Polynomial Mutation
void UNSGA::Reproducor::Mutate(Individual& individual)
{
	size_t scale = configuration_->scale;

//	std::vector<float, UNSGA::Allocator<float>> random(scale);
	std::vector<float> random(scale);
	std::vector<bool> labels(scale, false);

	vsRngUniform(VSL_RNG_METHOD_UNIFORM_STD, configuration_->stream, scale, &random[0], 0, 1);

	for (size_t i = 0; i < scale; ++i)
	{
		if (random[i] > 0.5)
		{
			random[i] = 1 - random[i];
			labels[i] = true;
		}
	}

	cblas_sscal(scale, 2, &random[0], 1);
	float coefficeint = 1 / (configuration_->mutation + 1);
	vsPowI(scale, &random[0], 1, &coefficeint, 0, &random[0], 1);
	coefficeint = 1;
	vsSubI(scale, &random[0], 1, &coefficeint, 0, &random[0], 1);

	for (size_t i = 0; i < scale; ++i)
	{
		if (labels[i])
		{
			random[i] = -random[i];
		}
	}

	vsSub(scale, configuration_->objective->upper(), configuration_->objective->lower(), &individual.decisions[0]);
	vsMul(scale, &random[0], &individual.decisions[0], &random[0]);
	vsAdd(scale, &random[0], configuration_->objective->lower(), &individual.decisions[0]);

//	mkl_free(random);
//	mkl_free(labels);
//	random = nullptr;
//	labels = nullptr;
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