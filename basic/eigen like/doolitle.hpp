#include "vector.hpp"
#include "matrix.hpp"

#ifndef _MATH_BASIC_EIGENLIKE_DOOLITLE_
#define _MATH_BASIC_EIGENLIKE_DOOLITLE_
template<typename T>
void Doolitle(const Matrix<T>& matrix, Vector<T>& vector) {
    size_t scale = matrix.row();
    size_t row = scale;
    size_t column = scale;

    Matrix<T> temporary = matrix;

    for(size_t r = 1; r < row; ++r) {
        temporary(r, 0) /= matrix(0, 0);
    }

    for(size_t k = 1; k < scale; ++k)
    {
        //update the upper triangle matrix
        for(size_t c = k; c < column; ++c)
        {
            T sum = 0;
            for(size_t  t = 0; t < k; ++t)
            {
                sum += temporary(k, t) * temporary(t, c);
            }

            temporary(k, c) = matrix(k, c) - sum;
        }

        //update the lower triangle matrix
        for(size_t r = k + 1; r < row; ++r)
        {
            T sum = 0;
            for(size_t t = 0; t < k; ++t)
            {
                sum += temporary(r, t) * temporary(t, k);
            }

            temporary(r, k) = (matrix(r, k) - sum) / temporary(k, k);
        }
    }

    //solve the lower triangle
    for(size_t i = 1; i < scale; ++i)
    {
        T sum = 0;
        for(size_t k = 0; k < i; ++k)
        {
            sum += temporary(i, k) * vector[k];
        }

        vector[i] -= sum;
    }

    //solve the upper triangle
    vector[row - 1] /= temporary(row -1, column - 1);

    for(int r = row - 2; r >= 0; --r)
    {
        T sum = 0;
        for(size_t k = r + 1; k < row; ++k)
        {
            sum += temporary(r, k) * vector[k];
        }

        vector[r] = (vector[r] - sum) / temporary(r, r);
    }
}
#endif // !_MATH_BASIC_EIGENLIKE_DOOLITLE_