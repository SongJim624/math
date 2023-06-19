#include "unsga.h"


UNSGA::Population::Individual::Individual(const Matrix<double>& decision, size_t dimension, size_t constraint) :
    decisions(decision),
    objectives(Matrix<double>(dimension)),
    voilations(Matrix<double>(constraint))
{
}