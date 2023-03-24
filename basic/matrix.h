/*
* This file is only for the float
*/

#include <memory>
#include <vector>
#include <mkl.h>

template<class T>
class Allocator
{
public:
	using value_type = T;
	Allocator() = default;

	template<class other>
	Allocator(const Allocator<other>&) {};

	void deallocate(T* pointer, const size_t length)
	{
		mkl_free(pointer);
	}

	T* allocate(size_t length)
	{
		return static_cast<T*>(mkl_malloc(length * sizeof(T), 64));
	}
};

class Matrix
{

};

template<typename L, typename R>
class operator_add
{
public:
    const L& lhs;
    const R& rhs;

public:
	auto calculate() const
	{}
};

template<typename L, typename R>
struct operator_substract
{

};

template<typename L, typename R>
struct operator_multiply
{

};

template<typename L, typename R>
struct operator_divide
{

};