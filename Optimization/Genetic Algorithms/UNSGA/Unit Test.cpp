#include "UNSGA.h"

class MyFunction : public Function
{
	virtual void function(float* decision, const long& decision_size, float* objective, const long& objective_size)
	{
		objective[0] = 0;

		for (long i = 0; i < decision_size; ++i)
		{
			objective[0] += pow(decision[i] - 1 / sqrt(decision_size), 2);
		}

		objective[0] = 1 - exp(-objective[0]);
		
		objective[1] = 0;

		for (long i = 0; i < decision_size; ++i)
		{
			objective[1] += pow(decision[i] + 1 / sqrt(decision_size), 2);
		}

		objective[1] = 1 - exp(-objective[1]);

	}
};

class NoConstriants : public Constraints
{
	virtual float constraints(float* decision, const long& decision_size, float* objective, const long& objective_size)
	{
		return 0;
	}
};

void main()
{
	Function* function = new MyFunction();
	Constraints* constraints = new NoConstriants();

	float* lower = new float[2]{ -1.0, -1.0 };
	float* upper = new float[2]{ 1.0, 1.0 };
	bool* integer = new bool[2]{ false, false };

	Optimizor* unsga = new UNSGA(function, constraints, 100, 2, 2, 100, upper, lower, integer);

	clock_t begin = clock();

	unsga->Optimize("HAHA.txt");

	clock_t end = clock();

	std::cout << float(end - begin) / 1000 << std::endl;
};