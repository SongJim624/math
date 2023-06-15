#include <vector>

#ifndef _MATH_BASIC_EIGENLIKE_DOOLITLE_
#define _MATH_BASIC_EIGENLIKE_DOOLITLE_
template<typename T, class allocator>
void Doolitle(const std::vector<T, allocator>& matrix, std::vector<T, allcoator>& vector)
{
    std::assert(void("size problem!"), std::pow(vector.size(), 2) == matrix.size());

    size_t scale = vector.size();
    size_t row = vector.size();
    size_t column = vector.size();

    std::vector<T, allocator> temporary = matrix;

    for(size_t r = 1; r < row; ++r)
    {
//        temporary(r, 0) /= matrix(0, 0);
        temporary[r * column] /= matrix[0];
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

template<typename T, class allocator>
void Doolitle(std::vector<T, allocator>& matrix, std::vector<T, allocator>& vector)
{

}
#endif // !_MATH_BASIC_EIGENLIKE_DOOLITLE_