#include "Matrix.h"

IMatrix::IMatrix(const long& r,const long& c) :
    row(r), column(c){}

IMatrix::~IMatrix(){}

long IMatrix::Row() const
{
    return row;
}

long IMatrix::Column() const
{
    return column;
}