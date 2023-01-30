#include "unsga.h"

void UNSGA::Reproducor::Initialize(VSLStreamStatePtr stream)
{
	stream_ = stream;
}

void UNSGA::Reproducor::Finalize()
{
	stream_ = nullptr;
}

//Simulated Binary Cross 
void UNSGA::Reproducor::Cross(const Individual& father, const Individual& mother, Individual& son, Individual& daughter)
{
	size_t decisions = information_->decision();

	float* random = (float*)mkl_malloc(decisions * sizeof(float), 64);
	float* weights = (float*)mkl_malloc(decisions * sizeof(float), 64);

	vsRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream_, decisions, random, 0, 1);

	for (long i = 0; i < decisions; ++i)
	{
		random[i] = (random[i] < 0.5) ? 2.0 * random[i] : 0.5 / (1.0 - random[i]);
	}

	float coefficient = 1 / (cross_ + 1);
	vsPowI(decisions, random, 1, &coefficient, 0, random, 1);

	coefficient = 1;
	vsAddI(decisions, &coefficient, 0, random, 1, weights, 1);
	vsMul(decisions, weights, father.decisions(), son.decisions());
	vsSubI(decisions, &coefficient, 0, random, 1, weights, 1);
	vsMul(decisions, weights, mother.decisions(), weights);
	vsAdd(decisions, weights, son.decisions(), son.decisions());

	vsSubI(decisions, &coefficient, 0, random, 1, weights, 1);
	vsMul(decisions, weights, father.decisions(), daughter.decisions());
	vsAddI(decisions, &coefficient, 0, random, 1, weights, 1);
	vsMul(decisions, weights, mother.decisions(), weights);
	vsAdd(decisions, weights, daughter.decisions(), son.decisions());

	cblas_sscal(decisions, 0.5, son.decisions(), 1);
	cblas_sscal(decisions, 0.5, daughter.decisions(), 1);

	mkl_free(random);
	mkl_free(weights);
	random = nullptr;
	weights = nullptr;
}

//Polynomial Mutation
void UNSGA::Reproducor::Mutate(Individual& individual)
{
	size_t decisions = information_->decision();

	float* random = (float *) mkl_malloc(decisions * sizeof(float), 64);
	bool* labels = (bool*)mkl_malloc(decisions * sizeof(bool), 64);

	std::fill(labels, labels + information_->decision(), false);
	vsRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream_, decisions, random, 0, 1);

	for (size_t i = 0; i < information_->decision(); ++i)
	{
		if (random[i] > 0.5)
		{
			random[i] = 1 - random[i];
			labels[i] = true;
		}
	}

	cblas_sscal(decisions, 2, random, 1);
	float coefficeint = 1 / (mutation_ + 1);
	vsPowI(decisions, random, 1, &coefficeint, 0, random, 1);
	coefficeint = 1;
	vsSubI(decisions, random, 1, &coefficeint, 0, random, 1);

	for (size_t i = 0; i < decisions; ++i)
	{
		if (labels[i])
		{
			random[i] = -random[i];
		}
	}

	vsSub(decisions, information_->upper(), information_->lower(), individual.decisions());
	vsMul(decisions, random, individual.decisions(), random);
	vsAdd(decisions, random, information_->lower(), individual.decisions());

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
	for (size_t i = 0; i < information_->decision(); ++i)
	{
		if (information_->integer()[i])
		{
			individual.decisions()[i] = std::round(individual.decisions()[i]);
		}
	}
}

void UNSGA::Reproducor::Reproduce(std::list<Individual*>& solution, std::list<Individual*>& population)
{
	std::list<Individual*> temporary;

	if (solution.size() % 2)
	{
		population.push_front(*solution.rbegin());
		solution.pop_back();
	}

	float* random = (float*)mkl_malloc(2 * sizeof(float), 64);

	auto father = solution.begin();
	auto mother = std::next(father);

	while (true)
	{
		if (population.empty())
		{
			break;
		}

		auto son = population.rbegin();
		auto daughter = std::next(son);

		Cross(**father, **mother, **son, **daughter);

		vsRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream_, 2, random, 0, 1);

		if (random[0] > threshold_)
		{
			Mutate(**son);
		}

		if (random[1] > threshold_)
		{
			Mutate(**daughter);
		}

		temporary.push_back(*son);
		temporary.push_back(*daughter);

		population.pop_back();
		population.pop_back();

		std::advance(father, 2);
		if (father == solution.end())
		{
			break;
		}

		std::advance(mother, 2);
	}

	solution.merge(temporary);
}