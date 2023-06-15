#include <iostream>
#include <fstream>
#include "unsga.hpp"

template<typename T>
class Objective : public Optimizor<T, std::allocator<T>>::Objective
{
private:
	size_t decisions_ = 2;

public:
	virtual void operator() (const T* decisions, T* objectives)
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

template<typename T>
class Constraint : public Optimizor<T, std::allocator<T>>::Constraint
{
public:
	virtual void operator() (const T* decisions, const T* objectives, T* voilations = nullptr) {}
};

template<typename T>
class Configuration : public Optimizor<T, std::allocator<T>>::Configuration
{
private:
	const size_t decisions_ = 2;
	const size_t objectives_ = 2;
	std::vector<T, std::allocator<T>> uppers_ = {1, 1};
	std::vector<T, std::allocator<T>> lowers_ = { -1, -1 };
	std::vector<T, std::allocator<T>> integers_ = { 0, 0 };

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

	virtual std::vector<T, std::allocator<T>> uppers() const
	{
		return uppers_;
	}

	virtual std::vector<T, std::allocator<T>> lowers() const
	{
		return lowers_;
	}

	virtual std::vector<T, std::allocator<T>> integers() const
	{
		return integers_;
	}
};

int main()
{
	std::unique_ptr < Optimizor<double, std::allocator<double>>::Configuration> config = std::make_unique<Configuration<double>>();
	config->objective = std::make_unique<Objective<double>>();
	config->constraint = std::make_unique<Constraint<double>>();

	(*config)["cross"] = 0.8f;
	(*config)["mutation"] = 0.8f;
	(*config)["maximum"] = size_t(100);
	(*config)["division"] = size_t(10);
	(*config)["population"] = size_t(1000);

	std::unique_ptr<Optimizor<double, std::allocator<double>>> optimizer = std::make_unique<UNSGA<double, std::allocator<double>>>();
	auto results = optimizer->Optimize(config.get());
	results->Write("results.txt");
	results = nullptr;
	std::cout << "hello" << std::endl;
	return 0;
};
