#include <vector>
#include <memory>
#include <type_traits>
#include <initializer_list>


#define use_MKL 0;

#if use_MKL
#include <mkl.h>
#endif 

namespace math
{
    template<class T>
    class mkl_allocator
    {
    public:
        using value_type = T;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        using pointer = value_type*;
        using const_pointer = const value_type*;
        using reference = value_type&;
        using const_reference = const value_type&;

        template<typename U>
        struct rebind
        {
            using other = mkl_allocator<U>;
        };

    public:
        mkl_allocator() = default;
        mkl_allocator(const mkl_allocator&) = default;

        template<class other>
        mkl_allocator(const mkl_allocator<other>&)
        {
        }

        ~mkl_allocator() = default;

    public:
        pointer allocate(size_t size, const T* hint = nullptr)
        {
            void* ptr = mkl_malloc(size * sizeof(value_type), 64);
            if (!ptr)
            {
                throw std::bad_alloc();
            }
            return static_cast<pointer>(ptr);
        }

        void deallocate(T* pointer, const size_t length)
        {
            mkl_free(pointer);
        }

    public:
        size_type max_size() const
        {
            return size_type(UINT_MAX / sizeof(value_type));
        }

        pointer address(reference x)
        {
            return (pointer)&x;
        }

        const_pointer const_address(const_reference x)
        {
            return (const_pointer)&x;
        }
    };

    template<typename T>
    #if use_MKL
    using allocator = mkl_allocator<T>;
    #else
    using allocator = std::allocator<T>;
    #endif
}

#ifndef _MATH_BASIC_VECTOR_
#define _MATH_BASIC_VECTOR_
namespace math
{
	template<typename T>
	concept VECTOR = requires(T vector, size_t pos)
	{
		T::base;
		vector.size();
		vector[pos];
	};

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