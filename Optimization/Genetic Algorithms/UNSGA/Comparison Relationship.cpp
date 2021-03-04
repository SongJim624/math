#include "UNSGA.h"

bool DominationCMP::operator()(Individual_UNSGA* A, Individual_UNSGA* B) const
{
	return A->dominated < B->dominated;
}

bool DistanceCMP::operator()(Individual_UNSGA* A, Individual_UNSGA* B) const
{
	return A->distance < B->distance;
}

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