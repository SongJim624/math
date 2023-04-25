#include "add.hpp"
#include "substract.hpp"
#include "multiply.hpp"
#include "divide.hpp"
#include "scale.hpp"
#include <concept>
#include <type_traits>

#ifndef _MATH_BASIC_EIGENLIKE_OPERATORS_
#define _MATH_BASIC_EIGENLIKE_OPERATORS_

template<class L, class R>
auto operator + (const L& lhs, const R& rhs) {
    return Add(lhs, rhs);
}

template<class L, class R>
auto operator - (const L& lhs, const R& rhs) {
    return Substract(lhs, rhs);
}

template<class L, class R>
auto operator * (const L& lhs, const R& rhs) {
    return Multiply(lhs, rhs);
}

template<class L, class R>
auto operator * (const L& lhs, const R& rhs) {
    return Multiply(lhs, rhs);
}

template<class L, class R>
concept ScaleVector
{}

template<class L, class R>
requires std::is_integral<T> || std::is_integral<R>
auto operator * (const L& lhs, const R& rhs) {
    return Scale(lhs, rhs);
}

template<class L, class R>
auto operator / (const L& lhs, const R& rhs) {
    return Divide(lhs, rhs);
}

/*
template<typename T>
T dot(const Vector<T>& lhs, const Vector<T>& rhs) {
	return 0;
}
*/
#endif //!_MATH_BASIC_EIGENLIKE_OPERATORS_