#include "UNSGA.h"

Point::Point(const std::list<float>& coor) 
	: rho(0), coordinate(new float[Individual::objective_size])
{
	float* copy = nullptr;
	copy = coordinate;

	for (auto iter : coor)
	{
		*copy++ = iter;
	}
};

Point::~Point()
{
	delete[] coordinate;
}