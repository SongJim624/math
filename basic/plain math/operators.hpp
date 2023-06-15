#include "add.hpp"
#include "substract.hpp"
#include "multiply.hpp"
#include "divide.hpp"
#include "scale.hpp"
#include "power.hpp"
#include <concepts>
#include <type_traits>
#include <vector>

#ifndef _MATH_BASIC_EIGENLIKE_OPERATORS_
#define _MATH_BASIC_EIGENLIKE_OPERATORS_
namespace Plain
{
    template<typename T>
    concept Expression = std::vector<T>;

    template<Expression L, Expression R>
    auto operator + (const L& lhs, const R& rhs)
    {
        return Add<L, R>(lhs, rhs);
    }

    template<Expression L, Expression R>
    auto operator - (const L& lhs, const R& rhs)
    {
        return Substract<L, R>(lhs, rhs);
    }

    template<Expression L, Expression R>
    auto operator * (const L& lhs, const R& rhs)
    {
        return Multiply<L, R>(lhs, rhs);
    }

    template<typename L, Expression R>
    auto operator * (const L& lhs, const R& rhs)
    {
        return Scale<L, R>(lhs, rhs);
    }

    template<Expression L, Expression R>
    auto operator / (const L& lhs, const R& rhs)
    {
        return Divide<L, R>(lhs, rhs);
    }

    template<Expression V, typename T>
    auto operator ^ (const V& base, const T& exponent)
    {
        return Power<T, V>(exponent, base);
    }

    template<typename T, class allocator>
    T dot(const std::vector<T, allocator>& lhs, const std::vector<T, allocator>& rhs)
    {
        T res = 0;
        for (size_t i = 0; i < lhs.size(); ++i) {
            res += lhs[i] * rhs[i];
        }

        return res;
    }
}
#endif //!_MATH_BASIC_EIGENLIKE_OPERATORS_