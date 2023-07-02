#include "unsga.h"

UNSGA::Population::Individual::Individual(size_t scale, size_t dimension, size_t constraint) :
    decisions(math::allocate(scale + dimension + constraint)),
    objectives(decisions + scale),
    voilations(constraint == 0 ? nullptr : decisions + scale + dimension)
{
}

UNSGA::Population::Individual::~Individual()
{
    free(decisions);
    decisions = nullptr;
    objectives = nullptr;
    voilations = nullptr;
}