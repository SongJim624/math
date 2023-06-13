#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "population.hpp"

#include "../../optimizer.h"

#ifndef _MATH_OPTIMIZTION_UNSGA_RESULTS_
#define _MATH_OPTIMIZTION_UNSGA_RESULTS_
template<typename T>
class Result : public Optimization::Result<T>
{
private:
	Series<T> elites_;

public:
	virtual void Write(const char* filename) const;
	virtual std::vector<std::vector<T>> objectives() const;
	virtual std::vector<std::vector<T>> decisions() const;

public:
	Result(const Series<T>& elites);
};

template<typename T>
Result<T>::Result(const Series<T>& elites) : elites_(elites)
{}

template<typename T>
std::vector<std::vector<T>> Result<T>::decisions() const 
{
	return {};
}

template<typename T>
std::vector<std::vector<T>> Result<T>::objectives() const
{
	return {};
}

template<typename T>
void Result<T>::Write(const char* filename) const
{
	std::ofstream file(filename);

	for (const auto& individual : elites_)
	{
		for (const auto& objective : individual->objectives)
		{
			file << objective << "\t";
		}
		file << std::endl;
	}

	file.close();
}
#endif //! _MATH_OPTIMIZTION_UNSGA_RESULTS_