#include "sparseEA.h"

Individual::Individual(size_t scale, size_t dimension, size_t constraint) :
    data_(create(scale + dimension + constraint + scale)),
    decisions(data_.get()),
    objectives(decisions + scale),
    voilations(objectives + dimension),
    masks(voilations + constraint)
{
}

Individual::~Individual()
{
}