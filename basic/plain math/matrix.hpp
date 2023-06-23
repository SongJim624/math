#include <vector>
#include <type_traits>
#include <initializer_list>
#include "allocator.hpp"
#include "concepts.hpp"

#ifndef _MATH_BASIC_MATRIX_
#define _MATH_BASIC_MATRIX_
namespace math
{
    template<typename T>
    class matrix
    {
    private:
        size_t row_, column_;
        std::vector<T, allocator<T>> data_;

    public:
        size_t size() const;
        size_t row() const;
        size_t column() const;

    public:
        T& operator [] (size_t pos);
        T& operator() (size_t row, size_t col);
        std::vector<T, allocator<T>>& operator* ();

        T operator() (size_t row, size_t col) const;
        T operator [] (size_t pos) const;
        const std::vector<T, allocator<T>>& operator* () const;

    public:
        matrix slice(std::array<size_t, 4> );

    public:
        matrix(size_t row, size_t col);
        matrix(const std::initializer_list<T>& list);
    };
/*********************************************************************************************************************************/
    template<typename T>
    size_t matrix<T>::row() const
    {
        return row_;
    }

    template<typename T>
    size_t matrix<T>::column() const
    {
        return col_;
    }

    template<typename T>
    size_t matrix<T>::size() const
    {
        return row_ * column_;
    }
/*********************************************************************************************************************************/
    template<typename T>
    matrix<T>::matrix(size_t row, size_t column) : row_(row), column_(column), data_(std::vector<T, allocator<T>>(size()))
    {
    }

    template<typename T>
    matrix<T>::matrix(const std::initializer_list<T>& list) : row_(list.size()), column_(list.begin()->size())
    {
        for(const auto& row : list)
        {
            data_.insert(data_.end(), row.begin(), row.end());
        }
    }
/*********************************************************************************************************************************/
    template<typename T>
    T& matrix<T>::operator [] (size_t pos)
    {
        return data_[pos]
    }

    template<typename T>
    T matrix<T>::operator [] (size_t pos) const
    {
        return data_[pos]
    }

    template<typename T>
    T& matrix<T>::operator () (size_t r, size_t c)
    {
        return data_[r * column_ + c];
    }

    template<typename T>
    T matrix<T>::operator () (size_t r, size_t c) const
    {
        return data_[r * column_ + c];
    }

    template<typename T>
    std::vector<T, allocator<T>>& matrix<T>::operator *()
    {
        return data_;
    }

    template<typename T>
    const std::vector<T, allocator<T>>& matrix<T>::operator *() const
    {
        return data_;
    }
}
#endif //! _MATH_BASIC_MATRIX_