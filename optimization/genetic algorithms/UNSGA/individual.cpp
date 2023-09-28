#include "unsga.h"

Individual::Individual(size_t scale, size_t dimension, size_t constraint) :
    data_(create(scale + dimension + constraint)),
    decisions(data_.get()), objectives(data_.get() + scale), voilations(data_.get() + scale + dimension)
{
}
