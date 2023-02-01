#include <iostream>
#include <fstream>
#include "unsga.h"

class Function : public Optimizer::Objective
{
private:
	const size_t decisions_ = 2;
	const size_t objectives_ = 2;
	const float upper_[2] = {-1, -1};
	const float lower_[2] = {1, 1};
	const bool integer_[2] = { false, false };

public:
	virtual size_t scale() const
	{
		return decisions_;
	}

	virtual size_t dimension() const
	{
		return objectives_;
	}

	virtual size_t constraints() const
	{
		return 0;
	}

	virtual const float* upper() const
	{
		return upper_;
	}

	virtual const float* lower() const
	{
		return lower_;
	}

	virtual const bool* integer() const
	{
		return integer_;
	}

	virtual void function(const float* decisions, float* objectives, float * voilations = nullptr)
	{
		objectives[0] = 0;

		for (long i = 0; i < decisions_; ++i)
		{
			objectives[0] += pow(decisions[i] - 1 / sqrt(decisions_), 2);
		}

		objectives[0] = 1 - exp(-objectives[0]);

		objectives[1] = 0;

		for (long i = 0; i < decisions_; ++i)
		{
			objectives[1] += pow(decisions[i] + 1 / sqrt(decisions_), 2);
		}

		objectives[1] = 1 - exp(-objectives[1]);

	}
};


void main()
{
	new Function();
	std::unique_ptr<Optimizer::Objective> objective = std::make_unique<Function>();

//	float* lower = new float[2]{ -1.0, -1.0 };
//	float* upper = new float[2]{ 1.0, 1.0 };
//	bool* integer = new bool[2]{ false, false };

	UNSGA::Configuration configuration;
	configuration.cross = 0.8;
	configuration.mutation = 0.8;
	configuration.threshold = 0.8;

	configuration.population = 500;
	configuration.division = 10;

	configuration.dimension = 2;
	configuration.scale = 2;
	configuration.objective = objective.get();

	std::unique_ptr<Optimizer> optimizer = std::make_unique<UNSGA>(configuration);

	clock_t begin = clock();
	optimizer->Optimize(objective.get());
	clock_t end = clock();

	std::cout << float(end - begin) / 1000 << std::endl;
};
