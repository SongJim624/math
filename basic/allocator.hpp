#include <mkl.h>

#ifndef _MATH_BASIC_ALLOCATOR_
#define _MATH_BASIC_ALLOCATOR_
namespace Math
{
    template<class T>
    class Allocator
    {
    public:
        using value_type = T;

        Allocator() = default;
        template<class other>
        Allocator(const Allocator<other>&) {};

        void deallocate(T* pointer, const size_t length);
        T* allocate(size_t length);
    };

    template<typename T>
    void Allocator<T>::deallocate(T* pointer, const size_t length)
    {
        mkl_free(pointer);
    }

    template<typename T>
	T* Allocator<T>::allocate(size_t length)
	{
		return static_cast<T*>(mkl_calloc(length * sizeof(T), 64));
	}
}
#endif //!_MATH_BASIC_ALLOCATOR_