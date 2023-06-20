#include <cassert>
#include <type_traits>
#include "plain.h"

#ifndef _MATH_BASIC_OPERATORS_
#define _MATH_BASIC_OPERATORS_
namespace math
{
	template<EXPRESSION L, EXPRESSION R>
	auto operator + (const L& lhs, const R& rhs)
	{
		return Operator(lhs, rhs, [](L::base l, R::base r) {return l + r; });
	}

	template<EXPRESSION L, EXPRESSION R>
	auto operator - (const L& lhs, const R& rhs)
	{
		return Operator(lhs, rhs, [](L::base l, R::base r) {return l - r; });
	}

	template<EXPRESSION L, EXPRESSION R>
	auto operator * (const L& lhs, const R& rhs)
	{
		return Operator(lhs, rhs, [](L::base l, R::base r) {return l * r; });
	}

	template<EXPRESSION L, EXPRESSION R>
	auto operator / (const L& lhs, const R& rhs)
	{
		return Operator(lhs, rhs, [](L::base l, R::base r) {return l / r; });
	}

	template<EXPRESSION L, EXPRESSION R>
	auto operator ^ (const L& lhs, const R& rhs)
	{
		return Operator(lhs, rhs, [](L::base l, R::base r) { return pow(l, r); });
	}

	template<SCALAR T, EXPRESSION E>
	auto operator ^ (const E& lhs, const T& scalar)
	{
		return Scalar(scalar, lhs, [](T l, T r) { return pow(r, l); });
	}

	template<SCALAR T, EXPRESSION E>
	auto operator * (const T& scalar, const E& rhs)
	{
		return Scalar(scalar, rhs, [](T l, T r) { return l * r; });
	}
}


/**********************************************************************************************************************************
scalar and vector level
*/
namespace math
{
	template<SCALAR T, VECTOR V, class O>
	class Scalar
	{
	public:
		using base = T;
		const T& scalar;
		const V& expression;
		const O& opr;

	public:
		Scalar(const T& scalar, const V& expression, const O& opr) : scalar(scalar), expression(expression), opr(opr)
		{
		}

		base operator[] (size_t index) const
		{
			return opr(scalar, expression[index]);
		}

		size_t size() const
		{
			return expression.size();
		}
	};

	template<SCALAR S, VECTOR V>
	auto operator + (const S& s, const V& V)
	{
		return Scalar(s, v, [](S scalar, S vector) { return scalar + vector; });
	}

	template<SCALAR S, VECTOR V>
	auto operator + (const S& s, const V& V)
	{
		return Scalar(s, v, [](S scalar, S vector) { return scalar + vector; });
	}

	template<SCALAR S, VECTOR V>
	auto operator - (const S& s, const V& V)
	{
		return Scalar(s, v, [](S scalar, S vector) { return scalar - vector; });
	}

	template<SCALAR S, VECTOR V>
	auto operator * (const S& s, const V& V)
	{
		return Scalar(s, v, [](S scalar, S vector) { return scalar * vector; });
	}

	template<SCALAR S, VECTOR V>
	auto operator / (const S& s, const V& V)
	{
		return Scalar(s, v, [](S scalar, S vector) { return scalar / vector; });
	}

	template<SCALAR S, VECTOR V>
	auto operator ^ (const S& s, const V& V)
	{
		return Scalar(s, v, [](S scalar, S vector) { return std::pow(scalar, vector); });
	}

	template<SCALAR S, VECTOR V>
	auto operator + (const S& s, const V& V)
	{
		return Scalar(s, v, [](S scalar, S vector) { return scalar + vector; });
	}
//---------------------------------------------------------------------------------------------------------------------------------
	template<SCALAR S, VECTOR V>
	auto operator + (const V& v, const S& s)
	{
		return Scalar(s, v, [](S scalar, S vector) { return scalar + vector; });
	}

	template<SCALAR S, VECTOR V>
	auto operator - (const V& v, const S& s)
	{
		return Scalar(s, v, [](S scalar, S vector) { return scalar - vector; });
	}

	template<SCALAR S, VECTOR V>
	auto operator * (const V& v, const S& s)
	{
		return Scalar(s, v, [](S scalar, S vector) { return scalar * vector; });
	}

	template<SCALAR S, VECTOR V>
	auto operator / (const V& v, const S& s)
	{
		return Scalar(s, v, [](S scalar, S vector) { return scalar / vector; });
	}

	template<SCALAR S, VECTOR V>
	auto operator ^ (const V& v, const S& s)
	{
		return Scalar(s, v, [](S scalar, S vector) { return std::pow(scalar, vector); });
	}
}
/**********************************************************************************************************************************
vector and vector level
*/
namespace math
{
	template<VECTOR L, VECTOR R, class O>
	class Vector
	{
	public:
		using base = L::base;

		const L& lhs;
		const R& rhs;
		const O& opr;

	public:
		Vector(const L& lhs, const R& rhs, const O& opr) : lhs(lhs), rhs(rhs), opr(opr)
		{
			assert((void("size not same!"), lhs.size()  ==  rhs.size()));
		}

		base operator[] (size_t index) const
		{
			return opr(lhs[index], rhs[index]);
		}

		size_t size() const
		{
			return lhs.size();
		}
	};

	template<VECTOR L, VECTOR R>
	auto operator + (const L& lhs, const R& rhs)
	{
		return Scalar(lhs, rhs, [](L::base l, R::base r) { return l + r; });
	}

	template<VECTOR L, VECTOR R>
	auto operator - (const L& lhs, const R& rhs)
	{
		return Scalar(lhs, rhs, [](L::base l, R::base r) { return l - r; });
	}
	template<VECTOR L, VECTOR R>
	auto operator * (const L& lhs, const R& rhs)
	{
		return Scalar(lhs, rhs, [](L::base l, R::base r) { return l * r; });
	}
	template<VECTOR L, VECTOR R>
	auto operator / (const L& lhs, const R& rhs)
	{
		return Scalar(lhs, rhs, [](L::base l, R::base r) { return l / r; });
	}

	template<VECTOR L, VECTOR R>
	auto operator ^ (const L& lhs, const R& rhs)
	{
		return Scalar(lhs, rhs, [](L::base l, R::base r) { return std::pow(l, r); });
	}
}
/**********************************************************************************************************************************
matrix and matrix level
*/
namespace math
{
	template<typename T>
	concept MATRIX = requires(T matrix, size_t r, size_t c)
	{
		requires VECTOR<T>;
		matrix.row();
		matrix.col();
		matrix(r, c);
	};

	template<MATRIX M>
	class Matrix
	{
	};

//TODO:
}
#endif //!_MATH_BASIC_EIGENLIKE_OPERATORS_
