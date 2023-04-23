#include <mkl.h>
#include <vector>
#include <cassert>

#ifndef _MATH_OPTIMIZATION_UNSGA_MATRIX_
#define _MATH_OPTIMIZATION_UNSGA_MATRIX_
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

template<class T, class Expression>
class Scale
{
public:
	T* operator () (T* results) const;
};

template<typename T, class Left, class Right>
class Add
{
public:
	T* operator () (T* results) const;
};

template<typename T, class Left, class Right>
class Substract
{
public:
	const Left& left;
	const Right& right;

public:
	T* operator() (T*);
};

template<typename T, class Left, class Right>
class Divide
{
public:
	const Left& left;
	const Right& right;

public:
	T* operator() (T*);
};

template<typename T, class Left, class Right>
class Multiply
{
public:
	const Left& left;
	const Right& right;

public:
	T* operator() (T*);
};

template<typename T, class Left, class Right>
class Power 
{
public:
	T* operator () (T* results) const;
};

template<typename T, class Expression>
class Logarithm
{
public:
	const Expression& expression;

public:
	T* operator () (T* results) const;
};

template<typename T, class Expression>
class Exponential
{
public:
	T* operator () (T* results) const;
};

//Vector for first and second level compute
template<typename T>
class Vector
{
private:
	size_t row_, column_;

public:
	size_t size() const { return std::max(row_, column_); }
};

//Matrix for third level compute
template<typename T>
class Matrix
{
private:
	size_t row_, column_;
	std::vector<T, Allocator<T>> data_;

public:
	T* address() { return &data_[0]; }

public:
	template<class Expression > Matrix(const Expression& expression);
	
	template<class Expression>Matrix& operator = (const Expression& expression) {
		expression(&data_[0]);
		return *this;
	}
};


template<typename T, class Left, class Right>
T* Substract<T, Left, Right>::operator() (T* results) {
	return results;
}
#endif //!_MATH_OPTIMIZATION_UNSGA_MATRIX_

