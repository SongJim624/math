#include "Matrix.h"

template<typename T>
Matrix<T> Dense<T>::operator + (const Matrix<T>& a)
{
    switch a.type
    {
    case Dense:
    {
        if float
        {
            add_float(this->data, a.data, row * col);
        }
        else
        {
            add_double(this->data, a.data, row * col);
        }
        
    }
    case Sparse:
    {}
    case Diag:
    {}
    }
}