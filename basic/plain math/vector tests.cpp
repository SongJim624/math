//#include "plain.h"
#include "vector.hpp"
#include <iostream>


int main()
{
	math::vector<double> first = { 3, 4, 5 };
//	math::vector<double> second = { 4, 5, 6 };
//	std::vector<double, math::allocator<double>> test(3, 0);
//	math::vector<double> add = first + second;
//	math::vector<double> pow = add ^ 2;

	for (const auto& value : *first)
	{
		std::cout << value << std::endl;
	}

	std::cout << "hello world!" << std::endl;
}