#include <iostream>
#include <fstream>
#include "unsga.h"

class Objective : public math::Optimizor::Objective
{
private:
	size_t decisions_ = 2;

public:
	virtual void operator() (const double * decisions, double * objectives, double * voilation)
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

int main()
{
	std::unique_ptr<math::Optimizor::Configuration> config = std::make_unique<math::Optimizor::Configuration>();
	config->objective = std::make_unique<Objective>();

	(*config)["cross"] = 0.8f;
	(*config)["mutation"] = 0.8f;
	(*config)["maximum"] = size_t(100);
	(*config)["division"] = size_t(10);
	(*config)["population"] = size_t(1000);

	std::unique_ptr<math::Optimizor> optimizer = std::make_unique<math::UNSGA>();
	const auto& results = optimizer->Optimize(*config);
	results.Write("results.txt");
	std::cout << "hello" << std::endl;
	return 0;
};
