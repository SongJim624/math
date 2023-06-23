#include "allocator.hpp"
#include "concepts.hpp"
#include <vector>
#include <type_traits>
#include <initializer_list>

#ifndef _MATH_BASIC_VECTOR_
#define _MATH_BASIC_VECTOR_
namespace math
{
    template<typename T>
    class vector
    {
    private:
        std::vector<T, allocator<T>> data_;

    public:
        using base = T;

    public:
        size_t size() const;
        T operator [] (size_t pos) const;
        T& operator [] (size_t pos);
        std::vector<T, allocator<T>>& operator* ();
        const std::vector<T, allocator<T>>& operator* () const;

    public:
        vector(const vector& vec) = default;
        vector(size_t length, T value);
        vector(std::initializer_list<T> list);
        template<VECTOR V> vector(const V& expression);
        template<std::input_iterator I> vector(const I& begin, const I& end);
    };
/**********************************************************************************************************************************
constructors
********************************************************************************************************************************/
    template<typename T>
    vector<T>::vector(std::initializer_list<T> list) : data_(std::vector<T, allocator<T>>(list))
    {
    }

    template<typename T>
    vector<T>::vector(size_t length, T value) : data_(std::vector<T, allocator<T>>(length, value))
    {
    }

    template<typename T>
    template<VECTOR V>
    vector<T>::vector(const V&  expression) : data_(std::vector<T, allocator<T>>(expression.size()))
    {
        for(size_t i = 0; i < data_.size(); ++i)
        {
            data_[i] = expression[i];
        }
    }

    template<typename T>
    template<std::input_iterator I>
    vector<T>::vector(const I& begin, const I& end) : data_(std::vector<T, allocator<T>>(begin, end))
    {
    }
/*********************************************************************************************************************************/
    template<typename T>
    T& vector<T>::operator[] (size_t pos)
    {
        return data_[pos];
    }

    template<typename T>
    T vector<T>::operator[] (size_t pos) const
    {
        return data_[pos];
    }

    template<typename T>
    std::vector<T, allocator<T>>& vector<T>::operator*()
    {
        return data_;
    }

    template<typename T>
    const std::vector<T, allocator<T>>& vector<T>::operator*() const
    {
        return data_;
    }
/*********************************************************************************************************************************/
}
#endif //! _MATH_BASIC_VECTOR_