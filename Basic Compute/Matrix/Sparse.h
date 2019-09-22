#include "Matrix.h"

template<typename T>
struct Term
{
    const long row, col;
    T value;
};

template<typename T>
class Sparse
{
private:
    long rwo, col;
    long num;
    Term* data;
};