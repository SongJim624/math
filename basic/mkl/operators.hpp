#include "matrix.h"
#include "add.hpp"

#ifndef _MATH_BASIC_OPERATORS_
#define _MATH_BASIC_OPERATORS_
namespace Math
{
    template<class L, class R, >
    auto operator + (const L& lhs, const R& rhs) {
        return Add<L, R>(lhs,rhs);
    }

    template<class L, class R>
    auto operator + (const L& rhs, const R& rhs) {

    }
}
#endif //!_MATH_BASIC_OPERATORS_
