#include "UNSGA.h"

Individual_UNSGA::Individual_UNSGA()
	: dominated(0), distance(0)
{}

Individual_UNSGA::~Individual_UNSGA()
{}

UNSGA::UNSGA(Function* function, Constraints* constraints, 
	const long& generation, const long decision_size, const long& objective_size,
	const long& nDiv, float * upper, float * lower, bool * integer)
	: generation(generation)
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

	A = new float* [objective_size];
	ideal = new float[objective_size];
	interception = new float[objective_size];

	for (long i = 0; i < objective_size; ++i)
	{
		A[i] = new float[objective_size];
		ideal[i] = +INFINITY;
	}
	
	std::list<std::list<float>> points = Reference(objective_size, nDiv);
	
	for (auto iter = points.begin(); iter != points.end(); ++iter)
	{
		Zr.push_back(new Point(*iter));
	}

	total_size = (points.size() % 2 + points.size()) * 2;

	individuals = std::vector<Individual_UNSGA*>(total_size, nullptr);

	for (auto& iter : individuals)
	{
		iter = new Individual_UNSGA();

		function->function(iter->individual.decisions, iter->individual.decision_size,
			iter->individual.objectives, iter->individual.objective_size);

		iter->individual.voilate =
			constraints->constraints(iter->individual.decisions, iter->individual.decision_size,
				iter->individual.objectives, iter->individual.objective_size);
	}
}

std::list<std::list<float>> UNSGA::Reference(const long& M, const long& N)
{
	if (M == 1)
	{
		return std::list<std::list<float>>(1, std::list<float>{float(N)});
	}
	else
	{
		std::list<std::list<float>> points;

		for (size_t i = 0; i <= N; ++i)
		{
			std::list<std::list<float>> res = Reference(M - 1, N - i);

			for (auto iter = res.begin(); iter != res.end(); ++iter)
			{
				iter->push_front(float(i));
			}

			points.insert(points.end(), res.begin(), res.end());
		}

		return points;
	}
}

void UNSGA::Optimize(const char* results_path)
{
	for (long i = 0; i < generation; ++i)
	{
		Select();
		Genetic();

		for (auto iter : individuals)
		{
			function->function(iter->individual.decisions, iter->individual.decision_size,
				iter->individual.objectives, iter->individual.objective_size);

			iter->individual.voilate =
				constraints->constraints(iter->individual.decisions, iter->individual.decision_size,
					iter->individual.objectives, iter->individual.objective_size);
		}
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

void UNSGA::Genetic()
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
