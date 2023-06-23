#include <concepts>
#include <type_traits>

#ifndef _MATH_BASIC_CONCEPTS_
#define _MATH_BASIC_CONCEPTS_
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
		matrix.column();
		matrix(row, col);
	};
}
#endif //! _MATH_BASIC_CONCEPTS_