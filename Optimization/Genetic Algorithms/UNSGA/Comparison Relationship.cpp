#include "UNSGA.h"

bool PointCMP::operator()(Point* A, Point* B) const
{
	if (A->individuals.size() == 0 && B->individuals.size())
	{
		return false;
	}

	if (A->individuals.size() == 0)
	{
		return false;
	}

	if (B->individuals.size() == 0)
	{
		return true;
	}

	return A->rho < B->rho;
}