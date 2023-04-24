#include <mkl.h>
#include <vector>
#include <cassert>

#ifndef _MATH_OPTIMIZATION_UNSGA_MATRIX_
#define _MATH_OPTIMIZATION_UNSGA_MATRIX_template<tyepename T>
template<typename T>
using Vector = std::vector<T>;

template<>
auto operator * ()

template<class L, class R>
auto operator + (const L& lhs, const R& rhs) {
	return Add(lhs, rhs);
}


#endif //!_MATH_OPTIMIZATION_UNSGA_MATRIX_

