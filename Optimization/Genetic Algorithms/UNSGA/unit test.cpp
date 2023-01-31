//#include "UNSGA.h"
#include <iostream>
#include <fstream>


class Function : public Information
{
private:
	const size_t decisions_ = 2;
	const size_t objectives_ = 2;


public:
	virtual size_t decision() const
	{
		return decisions_;
	}

	virtual size_t objective() const
	{
		return objectives_;
	}

	virtual void function(float* decisions, float* objectives)
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
//	std::unique_ptr<Information> information = std::make_unique<Function>();

//	float* lower = new float[2]{ -1.0, -1.0 };
//	float* upper = new float[2]{ 1.0, 1.0 };
//	bool* integer = new bool[2]{ false, false };

//	std::unique_ptr<Optimizer> optimizer = std::make_unique<UNSGA>(function, constraints, 100, 2, 2, 100, upper, lower, integer);

	clock_t begin = clock();
//	optimizer->Optimize(information.get());
	clock_t end = clock();

	std::cout << float(end - begin) / 1000 << std::endl;
};
