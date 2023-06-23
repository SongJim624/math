#include "mkl.h"
/*
* response for a + b
*	ax + y
* ax + by
*/
#ifndef _MATH_BASIC_ADD_
#define _MATH_BASIC_ADD_
namespace math
{
	template<typename T>
	concept SCALE requires(T scale)
	{
		T::value_type;
		scale.scalar;
		scale.vector;
	};

	template<SCALAR S, VECTOR V>
	class Scale
	{
	public:
		using value_type = V::value_type;

	public:
		const S& scalar;
		const V& vector;

	public:
		Scale(const S& scalar, const V& vector) : scalar(scalar), vector(vector)
		{
		}
	};

	template<SCALE S, VECTOR V>
	class AXPY
	{
	public:
		using value_type = V::value_type;
		using pointer = value_type*;

	public:
		const S& scale;
		const V& vector;
		pointer result;

	public:
		size_t size() const
		{
			return vector.size();
		}

	public:
		AXPY(const S& scale, const V& vector) : scale(scale), vector(vector), result(nullptr)
		{
		}

		~AXPY()
		{
			if(result)
			{
				mkl_free(result);
			}
		}

		pointer operator () ();
		{
			if(!result)
			{
				pointer = (pointer) mkl_malloc(size() * sizeof(value_type), 64);
			}

			if(std::is_same<value_type, double>)
			{
				cblas_daxpy(size(), scale.scalar, scale)
			}

			return result;
		}
	};



	template<SCALAR S, VECTOR V>
	auto operator * (const S& scalar, const V& vector)
	{
		return Scale(scalar, vector);
	}

	template<SCALAR S, VECTOR V>
	auto operator / (const V& vector, const S& scalar)
	{
		return Scale(1 / scalar, vector);
	}


	template<SCALE S, VECTOR V>
	auto operator + ();

	template<SCALE S, VECTOR V>
	auto operator - ();



	template<class L, class R>
	class Add
	{
	public:
		using base = L::base;

	public:
		const L& lhs;
		const R& rhs;

		base* address_ = nullptr;
	public:
		const base* address(base * results) const {
			return lhs(results);
		}

	public:
		size_t row() const { return lhs.row(); }
		size_t column() const { return lhs.column(); }

	public:
		Add(const L& lhs, const R& rhs) : lhs(lhs), rhs(rhs) {}

	public:
		base* operator () (base* results) const;
	};

	template<class L, class R>
	Add<L, R>::base*Add<L, R>::operator() (L::base* results) const {
		std::vector<L::base, Allocator<L::base>> temporary(row() * column());

		if (std::is_same<L::base, float>) {
			vsAdd(row() * column(), lhs(results), rhs(&temporary[0]), results);
		}

		if (std::is_same<L::base, double>) {
			vdAdd(row() * column(), lhs(results), rhs(&temporary[0]), results);
		}

		return results;
	}

	template<SCALAR L, SCALAR R>



	//---------------
	template<>
	float* Add<Matrix<float>, Matrix<float>>::operator() (float* results) const {
		vsAdd(row() * column(), lhs(results), rhs(results), results);
		return results;
	}

	template<>
	double* Add<Matrix<double>, Matrix<double>>::operator() (double* results) const {
		vdAdd(row() * column(), lhs(results), rhs(results), results);
		return results;
	}
	//----------------------------------
	template<>
	float* Add<Scale<float, Matrix<float>>, Matrix<float>>::operator() (float* results) const {
		cblas_scopy(row() * column(), rhs(results), 1, results, 1);
		cblas_saxpy(row() * column(), lhs.scalar, lhs.matrix(results), 1, results, 1);
		return results;
	}

	template<>
	double* Add<Scale<double, Matrix<double>>, Matrix<double>>::operator() (double* results) const {
		cblas_dcopy(row() * column(), rhs(results), 1, results, 1);
		cblas_daxpy(row() * column(), lhs.scalar, lhs.matrix(results), 1, results, 1);
		return results;
	}
	//--------------
	template<>
	float* Add<Matrix<float>, Scale<float, Matrix<float>>>::operator() (float* results) const {
		cblas_scopy(row() * column(), lhs(results), 1, results, 1);
		cblas_saxpy(row() * column(), rhs.scalar, rhs.matrix(results), 1, results, 1);
		return results;
	}

	template<>
	double* Add<Matrix<double>, Scale<double, Matrix<double>>>::operator() (double* results) const {
		cblas_dcopy(row() * column(), lhs(results), 1, results, 1);
		cblas_daxpy(row() * column(), rhs.scalar, rhs(results), 1, results, 1);
		return results;
	}
	//------------------
	template<>
	float* Add<Scale<float, Matrix<float>>, Scale<float, Matrix<float>>>::operator() (float* results) const {
		cblas_scopy(row() * column(), rhs.matrix(results), 1, results, 1);
		cblas_saxpby(row() * column(), lhs.scalar, lhs.matrix(results), 1, rhs.scalar, results, 1);
		return results;
	}

	template<>
	double* Add<Scale<double, Matrix<double>>, Scale<double, Matrix<double>>>::operator() (double* results) const {
		cblas_dcopy(row() * column(), rhs.matrix(results), 1, results, 1);
		cblas_daxpby(row() * column(), lhs.scalar, lhs.matrix(results), 1, rhs.scalar, results, 1);
		return results;
	}
}
#endif //!_MATH_BASIC_ADD_