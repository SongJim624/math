#include <vector>
#include <cassert>
#include "allocator.hpp"

#ifndef _MATH_BASIC_MATRIX_
#define _MATH_BASIC_MATRIX_
namespace Math
{
    template<typename T>
    class Matrix
    {
    private:
        size_t row_, column_;
        std::vector<T> data_;

    public:
        using base = typename T;

    public:
        size_t  row() const;
        size_t  column() const;

    public:
        const T* address() const { return &data_[0]; }

    public:
        T operator() (size_t row, size_t column) const;
        T& operator() (size_t row, size_t column);
        const T* operator() (T*) const;
    public:
        Matrix(size_t row, size_t column);

    //    template<typename S>
    //    Matrix(const S& matrix);

        template<typename S> Matrix(size_t row, size_t column, const S& data);
        template<class Expression> Matrix(const Expression& expression);
        template<class Expression>Matrix& operator = (const Expression& expression);
    };

    template<typename T>
    size_t Matrix<T>::row() const {
        return row_;
    }

    template<typename T>
    size_t Matrix<T>::column() const {
        return column_;
    }

    template<typename T>
    T Matrix<T>::operator() (size_t row, size_t column) const {
        return data_[row * column_ + column];
    }

    template<typename T>
    const T* Matrix<T>::operator() (T* pointer) const {
        return &data_[0];
    }

    template<typename T>
    T& Matrix<T>::operator() (size_t row, size_t column) {
        return data_[row * column_ + column];
    }

    template<typename T>
    Matrix<T>::Matrix(size_t row, size_t column) : row_(row), column_(column) {
        data_.resize(row_ * column_);
    }

    template<typename T>
    template<typename S>
    Matrix<T>::Matrix(size_t row, size_t column, const S& data) : row_(row), column_(column) {
        data_.assign(data.begin(), data.end());
    }

    template<typename T>
    template<typename Expression>
    Matrix<T>::Matrix(const Expression& expression) : row_(expression.row()), column_(expression.column()) {
        data_.resize(row_ * column_);
        expression(&data_[0]);
    }

    template<typename T>
    template<class Expression>
    Matrix<T>& Matrix<T>::operator = (const Expression& expression) {
        row_ = expression.row();
        column_ = expression.column();
        data_.resize(row_ * column_);
        expression(&data_[0]);
        return *this;
    }

    /*
    template<typename T>
    template<typename S>
    Matrix<T>::Matrix(const S& matrix) : row_(matrix.size()), column_(S.begin()->size()){
        auto iter = S.begin();

        while (iter++ != S.end()) {
            assert(iter->size() == column_, "input is not a matrix!");
            data_.insert(data_.end(), iter->begin(), iter->end());
        }
    }
    */
}
#endif //!_MATH_BASIC_MATRIX_
