#include "Genetic Operation.h"
//Simulated Binary Cross 
void Genetic_Operation::Cross(Individual& father, Individual& mother,
	Individual& son, Individual& daughter)
{
	for (long i = 0; i < Individual::decision_size; ++i)
	{
		float r = (float)rand() / RAND_MAX;

		float bq = (r < 0.5) ?
			pow(2.0 * r, 1.0 / (mu + 1.0)) :
			pow(0.5 / (1.0 - r), 1.0 / (mu + 1.0));

		float value_son = 0.5 * ((1.0 + bq) * father.decisions[i] + (1.0 - bq) * mother.decisions[i]);
		float value_daughter = 0.5 * ((1.0 - bq) * father.decisions[i] + (1.0 + bq) * mother.decisions[i]);

		value_son = fmaxf(fminf(value_son, Individual::upper[i]), Individual::lower[i]);

		value_daughter = fmaxf(fminf(value_son, Individual::upper[i]), Individual::lower[i]);

		if (Individual::integer[i])
		{
			value_son = (int)value_son;
			value_daughter = (int)value_daughter;
		}

		son.decisions[i] = value_son;
		daughter.decisions[i] = value_daughter;
	}
}
//Polynomial Mutation
void Genetic_Operation::Mutation(Individual& A)
{
	for (long i = 0; i < Individual::decision_size; ++i)
	{

		float r = (float)rand() / RAND_MAX;

		float value = (r < 0.5) ?
			powf(2.0f * r, 1 / (mum + 1)) - 1 :
			1 - powf(2.0f * (1 - r), 1 / (mum + 1));

		value= fmaxf(fminf(value, Individual::upper[i]), Individual::lower[i]);

		if (Individual::integer[i])
		{
			value = (int)value;
		}

		A.decisions[i] = value;
	}
}