#include <iostream>
#include <fstream>
#include "unsga.h"

class Objective : public Optimization::Objective<float>
{
private:
	size_t decisions_ = 2;

public:
	virtual void operator() (const float* decisions, float* objectives)
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

class Constraint : public Optimization::Constraint<float>
{
public:
	virtual void operator() (const float* decisions, const float* objectives, float* voilations = nullptr) {}
};

class Configurations : public Optimization::Configuration<float>
{
private:
	const size_t decisions_ = 2;
	const size_t objectives_ = 2;
	const float upper_[2] = {-1, -1};
	const float lower_[2] = {1, 1};
	const bool integer_[2] = { false, false };

public:
	virtual size_t scales() const
	{
		return decisions_;
	}

	virtual size_t dimensions() const
	{
		return objectives_;
	}

	virtual size_t constraints() const
	{
		return 0;
	}

	virtual const float* uppers() const
	{
		return upper_;
	}

	virtual const float* lowers() const
	{
		return lower_;
	}

	virtual const bool* integers() const
	{
		return integer_;
	}
};


void main()
{
	std::unique_ptr<Optimization::Configuration<float>> config = std::make_unique<Configurations>();
	config->objective = std::make_unique<Objective>();
	config->constraint = std::make_unique<Constraint>();

	(*config)["cross"] = 0.8f;
	(*config)["mutation"] = 0.8f;
	(*config)["maximum"] = size_t(10000);
	(*config)["division"] = size_t(10);
	(*config)["population"] = size_t(10000);

	std::unique_ptr<Optimization::Optimizer<float>> optimizer = std::make_unique<UNSGA<float>>();
	auto results = optimizer->Optimize(config.get());
	results->Write("results.txt");
	/*
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
	configuration.maximum = 5000;

	configuration.dimension = 2;
	configuration.scale = 2;
	configuration.objective = objective.get();

	std::unique_ptr<Optimizer> optimizer = std::make_unique<UNSGA>(configuration);

	clock_t begin = clock();
	optimizer->Optimize(objective.get());
	clock_t end = clock();

	std::cout << float(end - begin) / 1000 << std::endl;
	*/
	std::cout << "hello" << std::endl;
};
