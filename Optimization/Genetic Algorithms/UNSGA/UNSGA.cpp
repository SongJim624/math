#include "unsga.h"

std::list<std::list<float>> Reference(size_t objectives, size_t divisions)
{
	if (objectives == 1)
	{
		return std::list<std::list<float>>{std::list<float>{divisions}};
	}
	else
	{
		std::list<std::list<float>> points;
		for (size_t i = 0; i <= divisions; ++i)
		{
			std::list<std::list<float>> res = Reference(objectives - 1, divisions - i);

			for (auto iter = res.begin(); iter != res.end(); ++iter)
			{
				iter->push_front(i);
			}

			points.splice(points.end(), res);
		}

		return points;
	}
}

UNSGA::UNSGA(Function* function, Constraints* constraints,
	size_t decisions, size_t objectives, size_t maximum, size_t division, const float * upper, const float * lower, const bool * integer)
{

	srand((unsigned)time(NULL));

	genetic = new Genetic_Operation(0.5, 0.5);
	this->function = function;
	this->constraints = constraints;

	Individual::decision_size = decision_size;
	Individual::objective_size = objective_size;

	Individual::upper = new float[decision_size];
	Individual::lower = new float[decision_size];
	Individual::integer = new bool[decision_size];

	for (long i = 0; i < decision_size; ++i)
	{
		Individual::upper[i] = upper[i];
		Individual::lower[i] = lower[i];
		Individual::integer[i] = integer[i];
	}

//	matrix_ = mkl_malloc(objectives * objectives * sizeof(float), 64);
//	ideal_ = mkl_malloc(objectives * sizeof(float), 64);
//	interception_ = mkl_malloc(objectives * sizeof(float), 64);

	for (auto& iter : Reference(Individual::size(), divisions))
	{
		Zr.push_back(new Point(*iter));
	}

	for(size_t i = 0; i < (points.size() % 2 + points.size()) * 2; ++i)
	{
		population.push_back(new Individual());
	}
}

void UNSGA::Optimize(const char* results_path)
{
	std::list<Individual*> population;

	for (size_t generation = 0; generation < maximum_; ++generation)
	{
		Select(population_);
		Genetic(population);
	}

	if (results_path)
	{
		Write(results_path);
	}
}

void UNSGA::Write(const char* results_path)
{
	std::ofstream file(results_path);

	for (auto iter : solution)
	{
		for (long i = 0; i < Individual::decision_size; ++i)
		{
			file << iter->individual.decisions[i] << "\t";
		}

		for (long i = 0; i < Individual::objective_size; ++i)
		{
			file << iter->individual.objectives[i] << "\t";
		}

		file << std::endl;
	}
}

void UNSGA::Genetic(std::list<Individual*>& population)
{
	auto iter = solution.begin();

	while (iter != solution.end())
	{
		Individual_UNSGA* father = *iter++;
		Individual_UNSGA* mother = *iter++;

		Individual_UNSGA* son = *extend.begin();
		extend.pop_front();

		Individual_UNSGA* daughter = *extend.begin();
		extend.pop_front();

		genetic->Cross(father->individual, mother->individual, son->individual, daughter->individual);

		//if()
		genetic->Mutation(son->individual);
	}
}

UNSGA::~UNSGA()
{
	delete[] Individual::upper;
	delete[] Individual::lower;
	delete[] Individual::integer;

	delete[] ideal;
	delete[] interception;

	for (long i = 0; i < Individual::objective_size; ++i)
	{
		delete[] A[i];
	}

	delete[] A;

	for (auto iter : Zr)
	{
		delete iter;
	}

	for (auto iter : individuals)
	{
		delete iter;
	}

	delete genetic;
}
