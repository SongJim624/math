#include <vector>

#ifndef _MATH_BASIC_EIGENLIKE_MATRIX_
#define _MATH_BASIC_EIGENLIKE_MATRIX_
template<typename T>
class Matrix
{
private:
    size_t row_, column_;
    std::vector<T> data_;

public:
    T& operator () (size_t row, size_t column);
    T operator () (size_t row, size_t column) const;
};
#endif //!_MATH_BASIC_EIGENLIKE_MATRIX_