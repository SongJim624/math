#include <vector>

#ifndef _MATH_BASIC_EIGENLIKE_MATRIX_
#define _MATH_BASIC_EIGENLIKE_MATRIX_

template <typename T>
using Vector = std::vector<T>;

template<typename T>
class Matrix
{
private:
    std::vector<T> data_;
    size_t row_, column_;

public:
    T& operator () (size_t row, size_t column);
    T operator () (size_t row, size_t column) const;

    size_t row() const;
    size_t column() const;

public:
    Matrix(size_t row, size_t column);
    Matrix(const std::vector<std::vector<T>>& matrix);
    Matrix(const std::vector<Vector<T>>& matrix);
    Matrix(size_t row, size_t column, const Vector<T>& data);
};

template<typename T>
Matrix<T>::Matrix(size_t row, size_t column) : row_(row), column_(column)
{
    data_.resize(row * column);
}

template<typename T>
Matrix<T>::Matrix(size_t row, size_t column, const Vector<T>& data) : row_(row), column_(column), data_(data)
{
}

template<typename T>
Matrix<T>::Matrix(const std::vector<std::vector<T>>& matrix)
{
    row_ = matrix.size();
    column_ = matrix[0].size();

    for(size_t r = 0; r < row_; ++r)
    {
        assert(matrix[r].size() == column_ && "columns of the matrix are not same!");
        data_.insert(data_.end(), matrix[r].begin(), matrix[r].end());
    }
}

template<typename T>
Matrix<T>::Matrix(const std::vector<Vector<T>>& matrix)
{
    row_ = matrix.size();
    column_ = matrix[0].size();

    for (size_t r = 0; r < row_; ++r)
    {
        assert(matrix[r].size() == column_ && "columns of the matrix are not same!");
        data_.append(matrix[r]);
    }
}

template<typename T>
size_t Matrix<T>::row() const
{
    return row_;
}

template<typename T>
size_t Matrix<T>::column() const
{
    return column_;
}

template<typename T>
T& Matrix<T>::operator() (size_t row, size_t column)
{
    return data_[row * column_ + column];
}

template<typename T>
T Matrix<T>::operator() (size_t row, size_t column) const
{
    return data_[row * column_ + column];
}
#endif //!_MATH_BASIC_EIGENLIKE_MATRIX_