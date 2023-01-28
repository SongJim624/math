#include "UNSGA.h"

Point::Point(const std::list<float>& coordinate)
	: rho(0), coordinate_(nullptr)
{
	coordinate_ = mkl_malloc(coordinate.size() * sizeof(float), 64);

	for (auto iter : coordinate)
	{
		*coordinate++ = *iter;
	}
};

Point::~Point()
{
	mkl_free(coordinate_);
}