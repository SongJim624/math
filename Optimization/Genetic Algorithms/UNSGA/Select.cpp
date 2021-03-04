#include "UNSGA.h"

void Doolittle(float ** A, float* b, const long& length)
{
	for (long i = 0; i < length; ++i)
	{
		A[0][i] = A[0][i];
		A[i][0] /= A[0][0];
	}

	long r = 1;
	for (; r < length - 1; ++r)
	{
		long i = r;
		float sum = 0.0;
		for (long k = 0; k < r; ++k)
		{
			sum += A[r][k] * A[k][i];
		}
		A[r][i] = A[r][i] - sum;
		++i;

		for (; i < length; ++i)
		{
			sum = 0.0;
			for (size_t k = 0; k < r; ++k)
			{
				sum += A[r][k] * A[k][i];
			}

			A[r][i] -= sum;

			sum = 0;
			for (size_t k = 0; k < r; ++k)
			{
				sum += A[i][k] * A[k][r];
			}

			A[i][r] = (A[i][r] - sum) / A[r][r];
		}
	}

	float sum = 0.0;
	for (size_t i = 0; i < r; ++i)
	{
		sum += A[r][i] * A[i][r];
	}
	A[r][r] -= sum;

	for (size_t i = 1; i < length; ++i)
	{
		sum = 0;

		for (size_t k = 0; k < i; ++k)
		{
			sum += A[i][k] * b[k];
		}

		b[i] -= sum;
	}

	b[length - 1] /= A[length - 1][length - 1];

	for (int i = length - 2; i >= 0; --i)
	{
		sum = 0.0;
		for (size_t k = i + 1; k < length; ++k)
		{
			sum += A[i][k] * b[k];
		}

		b[i] = (b[i] - sum) / A[i][i];
	}
}

float Distance(float * A, float * B, const long& length)
{
	float a = 0, b = 0, c = 0;
	
	for (long i = 0; i < length; ++i)
	{
		a += A[i] * A[i];
		b += B[i] * B[i];
		c += A[i] * B[i];
	}

	return sqrt(a - c / b);
}

void UNSGA::Ideal_Point(std::list<Individual_UNSGA*>& top)
{	
/*
* The value of the ideal point is not initialized to the +Infinity
* because the ideal point is the minimum value of the last iteration
* only when the value of the current iteration is smaller than the last iteration
* could the ideal point be updated 
*/
	for (auto iter : top)
	{
		for (long i = 0; i < Individual::objective_size; ++i)
		{
			ideal[i] = fminf(ideal[i], iter->individual.objectives[i]);
		}
	}
}

void UNSGA::Interception(std::list<Individual_UNSGA*>& bottom)
{
	std::vector<float> ASF(Individual::objective_size, +INFINITY);
	std::vector<Individual_UNSGA*> extreme(Individual::objective_size, nullptr);

	for (auto iter : bottom)
	{
		for (long i = 0; i < Individual::objective_size; ++i)
		{
			iter->cost[i] = iter->individual.objectives[i] - ideal[i];
		}

		for (long i = 0; i < Individual::objective_size; ++i)
		{
			float asf = -INFINITY;

			for (long j = 0; j < Individual::objective_size; ++j)
			{
				asf = fmaxf(iter->cost[j] / ((j == i) * 1.0f + (j != i) * 1e-6f), asf);
			}

			if (ASF[i] > asf)
			{
				ASF[i] = asf;
				extreme[i] = iter;
			}
		}
	}

	for (long i = 0; i < Individual::objective_size; ++i)
	{
		interception[i] = 1.0f;

		for (long j = 0; j < Individual::objective_size; ++j)
		{
			A[i][j] = extreme[i]->individual.objectives[j];
		}
	}

	Doolittle(A, interception, Individual::objective_size);

	for (long i = 0; i < Individual::objective_size; ++i)
	{
		interception[i] = 1.0f / interception[i];
	}
}

void UNSGA::Associate(std::list<Individual_UNSGA*>& critical)
{
	for (auto& iter : Zr)
	{
		iter->rho = 0;
		iter->individuals.clear();
	}

	for (auto iter : solution)
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

void UNSGA::Niche(std::list<Individual_UNSGA*>& critical)
{
	long N = total_size / 2 - solution.size();

/*
* The choice of Zr should be random when the number of the points whose rho equals 0 is larger than one
* Here the first one of the list is used directly
* because the rho would be one after the individual associated with the point pushed into solution
* the later whose rho is also 0 would become the begin of the list
*/

	for (long i = 0; i < N; ++i)
	{
		Zr.sort(PointCMP());
		Point* point = *Zr.begin();
		point->individuals.sort(DistanceCMP());
		solution.push_back(*point->individuals.begin());
		point->rho++;
		point->individuals.pop_front();
		critical.remove(*solution.rbegin());
	}
}

void UNSGA::Normalize(std::list<Individual_UNSGA*>& selected)
{
	for (auto& iter : selected)
	{
		for (long i = 0; i < Individual::objective_size; ++i) 
		{
			iter->individual.objectives[i] -= ideal[i];
			iter->individual.objectives[i] /= interception[i] - ideal[i];
		}
	}
}

void UNSGA::Select(std::list<Individual_UNSGA*>& critical)
{
	Interception(critical);

	Normalize(solution);
	Normalize(critical);

	Associate(solution);
	Associate(critical);

	Niche(critical);
}

void UNSGA::Select()
{
	solution.clear();

	std::list<std::list<Individual_UNSGA*>> layers = Sort();
	
	Ideal_Point(*layers.begin());

	while (solution.size() + layers.begin()->size() < total_size / 2)
	{
		solution.insert(solution.end(), layers.begin()->begin(), layers.begin()->end());
		layers.pop_front();
	}

	if (solution.size() + layers.begin()->size() > total_size / 2)
	{
		Select(*layers.begin());
	}
	else
	{
		solution.insert(solution.end(), layers.begin()->begin(), layers.begin()->end());
		layers.pop_front();
	}

	while (layers.size() != 0)
	{
		extend.insert(extend.end(), layers.begin()->begin(), layers.begin()->end());
		layers.pop_front();
	}
}
