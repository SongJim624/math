#include "Interpolation.h"
#include "Interpolation.cpp"
#include "Largrange.cpp"
#include "Hermit.cpp"

#include <iostream>
void main()
{
	float* X = new float[4]{ 1, 2, 3, 4 };
	float* Y = new float[4]{ 1, 2, 3, 4 };
	float* dY = new float[4]{ 1, 1, 1, 1 };

	Interpolation<float> * largrange = new Largrange<float>(X, Y, 4);
	std::cout << largrange->Interpolate(3.69) << std::endl;

	Interpolation<float>* hermite = new Hermite<float>(X, Y, dY, 4);
	std::cout << hermite->Interpolate(3.56) << std::endl;

	delete[] X;
	delete[] Y;

	delete largrange;
	delete hermite;
};