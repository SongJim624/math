#include "sparseEA.h"

Individual::Individual(size_t scale, size_t dimension, size_t constraint) :
    data_(create(scale + dimension + constraint + scale)),
    decisions(data_.get()),
    objectives(decisions + scale),
    voilations(objectives + dimension),
    masks(voilations + constraint)
{
    std::fill(decisions, decisions + scale + dimension + constraint + scale, 0.0);
}

Individual::~Individual()
{
    data_.reset(nullptr);
    decisions = nullptr;
    objectives = nullptr;
    voilations = nullptr;
    masks = nullptr;
}