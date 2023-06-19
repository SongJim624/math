#include <cassert>

#ifndef _MATH_BASIC_OPERATOR_
#define _MATH_BASIC_OPERATOR_
namespace math
{
	template<typename T>
	concept SCALAR = requires(T scalar)
	{
		std::is_arithmetic<T>::value == true;
	};

	template<typename T>
	concept VECTOR = requires(T vector, size_t pos)
	{
		T::base;
		vector.size();
		vector[pos];
	};

	template<typename T>
	concept MATRIX = requires(T matrix, size_t row, size_t col)
	{
		requires VECTOR<T>;
		matrix.row();
		matrix.col();
		matrix(row, col);
	};

	template<typename T>
	concept EXPRESSION = requires(T expression)
	{
		expression.size();
//		requires VECTOR<T>;
	};

	template<SCALAR T, EXPRESSION E, class O>
	class Scalar
	{
	public:
		using base = T;
		const T& scalar;
		const E& expression;
		const O& opr;

	public:
		Scalar(const T& scalar, const E& expression, const O& opr) : scalar(scalar), expression(expression), opr(opr)
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


	template<EXPRESSION L, EXPRESSION R, class O>
	class Operator
	{
	private:
		size_t size_;

	public:
		using base = L::base;

		const L& lhs;
		const R& rhs;
		const O& opr;

	public:
		Operator(const L& lhs, const R& rhs, const O& opr) : lhs(lhs), rhs(rhs), opr(opr)
		{
			assert((void("size not same!"), lhs.size()  ==  rhs.size()));
			size_ = lhs.size();
		}

		base operator[] (size_t index) const
		{
			return opr(lhs[index], rhs[index]);
		}

		size_t size() const
		{
			return size_;
		}
	};

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
#endif //!_MATH_BASIC_EIGENLIKE_ADD_
