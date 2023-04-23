#include <mkl.h>
#include <vector>
#include <cassert>
#include "dense matrix.hpp"
#include "add.hpp"

#ifndef _MATH_OPTIMIZATION_UNSGA_MATRIX_
#define _MATH_OPTIMIZATION_UNSGA_MATRIX_
namespace Math
{
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

	template<class Left, class Right>
	auto operator + (const Left& lhs, const Right& rhs) {
		return Add<Left, Right>(lhs, rhs);
	}

	template<typename T, class M>
	auto operator * (const T& scalar, const M& matrix) {
		return Scale<T, M>(scalar, matrix);
	}

	template<typename T, class Left, class Right>
	T* Substract<T, Left, Right>::operator() (T* results) {
		return results;
	}
}
#endif //!_MATH_OPTIMIZATION_UNSGA_MATRIX_