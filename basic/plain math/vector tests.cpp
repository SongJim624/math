#include "plain.h"
#include <iostream>


int main()
{
	math::vector<double> first = { 3, 4, 5 };
	math::vector<double> second = { 4, 5, 6 };
	math::vector<double> add = first + second;
	math::vector<double> pow = add ^ 2;

	std::cout << "hello world!" << std::endl;
}