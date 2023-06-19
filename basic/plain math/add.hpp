#include <cassert>

#ifndef _MATH_BASIC_OPERATOR_
#define _MATH_BASIC_OPERATOR_
template<typename T>
concept VECTOR = requires(T vector, size_t pos)
{
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
	requires VECTOR<T>;
	T::base;
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
		assert((void("size not same!", lhs.size() == rhs.size())));
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
	return Operator(lhs, rhs, [](L::base l, R::base r) {return l + r})
}

template<EXPRESSION L, EXPRESSION R>
auto operator - (const L& lhs, const R& rhs)
{
	return Operator(lhs, rhs, [](L::base l, R::base r) {return l - r})
}

template<EXPRESSION L, EXPRESSION R>
auto operator * (const L& lhs, const R& rhs)
{
	return Operator(lhs, rhs, [](L::base l, R::base r) {return l * r})
}

template<EXPRESSION L, EXPRESSION R>
auto operator / (const L& lhs, const R& rhs)
{
	return Operator(lhs, rhs, [](L::base l, R::base r) {return l / r})
}
#endif //!_MATH_BASIC_EIGENLIKE_ADD_
