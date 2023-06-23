#define use_MKL 0;

#include <memory>
#if use_MKL
#include <mkl.h>
#endif

#ifndef _MATH_BASIC_ALLOCATOR_
#define _MATH_BASIC_ALLOCATOR_
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
#endif //! _MATH_BASIC_ALLOCATOR_