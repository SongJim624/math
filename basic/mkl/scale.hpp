#include <mkl.h>
#include <type_traits>
#include "dense matrix.hpp"

#ifndef _MATH_BASIC_SCALE_
#define _MATH_BASIC_SCALE_
namespace Math
{
	template<typename T, class M>
	class Scale
	{
    public:
        using base = T;
	public:
		const T& scalar;
		const M& matrix;

		base* address_ = nullptr;
	public:
		const base* address(base * results) const {
			return cblas_sscal(row() * column(), scalar, results, 1);
		}

	public:
		size_t row() const { return matrix.row(); }
		size_t column() const { return matrix.column(); }

	public:
		Scale(const T& scalar, const M& matrix) : scalar(scalar), matrix(matrix) {}

	public:
//		template<typename = typename std::enable_if<std::is_same<R, Matrix<base>>>>
		base* operator () (base* results) const { return results; };
	};
}
#endif //!_MATH_BASIC_SCALE_