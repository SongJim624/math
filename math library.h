//#define __USING_MKL__

#include <tuple>
#include <utility>
#include <cmath>
#include <memory>
#include <functional>

#ifdef __USING_MKL__
#include <mkl.h>
#endif

#ifndef _MATH_BLAS_
#define _MATH_BLAS_
//  allocator of array
namespace math
{
    template<typename T>
    using pointer = std::unique_ptr<T[], void(*)(void*)>;

    template<typename T>
    pointer<T> allocate(size_t size)
    {
    #if defined __INTEL_MKL__
        return pointer<T>((T*)MKL_calloc(size, sizeof(T), 64), &MKL_free);
    #else
        return pointer<T>((T*)std::calloc(size, sizeof(T)), &std::free);
    #endif
    }

    template<typename T, typename Operation>
    void operate(Operation operation, size_t size, const T* operand, size_t oinc, T* results, size_t inc)
    {
        for (size_t i = 0; i < size; ++i)
        {
            results[i * inc] = operation(operand[i * oinc]);
        }
    }

    template<typename T, typename Operation>
    void operate(Operation operation, size_t size, const T* lhs, size_t linc, const T* rhs, size_t rinc, T* results, size_t inc)
    {
        for (size_t i = 0; i < size; ++i)
        {
            results[i * inc] = operation(lhs[i * linc], rhs[i * rinc]);
        }
    }
};

//  copy fucntion
namespace math
{
    template<typename T>
    void copy(size_t size, const T* source, size_t sinc, T* destination, size_t dinc)
    {
        operate([](T value) { return value; }, size, source, sinc,  destination, dinc);
    }

#ifdef  __INTEL_MKL__
    template<>
    void copy(size_t size, const double* source, size_t sinc, double* destination, size_t dinc)
    {
        cblas_dcopy(size, source, sinc, destination, dinc);
    }

    template<>
    void copy(size_t size, const float* source, size_t sinc, float* destination, size_t dinc)
    {
        cblas_scopy(size, source, sinc, destination, dinc);
    }
#endif
}

//  add
namespace math
{
    template<typename T>
    void add(size_t size, const T* lhs, size_t linc, const T* rhs, size_t rinc, T* res, size_t inc)
    {
        operate(std::plus<T>(), size, lhs, linc, rhs, rinc, res, inc);
    }

#ifdef __INTEL_MKL__
    template<>
    void add(size_t size, const double* lhs, size_t linc, const double* rhs, size_t rinc, double* res, size_t inc)
    {
        vdAddI(size, lhs, linc, rhs, rinc, res, inc);
    }

    template<>
    void add(size_t size, const float* lhs, size_t linc, const float* rhs, size_t rinc, float* res, size_t inc)
    {
        vsAddI(size, lhs, linc, rhs, rinc, res, inc);
    }
#endif

    template<typename T>
    void add(size_t size, T lhs, const T* rhs, size_t rinc, T* res, size_t inc)
    {
        add(size, &lhs, 0 , rhs, rinc, res, inc);
    }

    template<typename T>
    void add(size_t size, const T* lhs, size_t linc, T rhs, T* res, size_t inc)
    {
        add(size, lhs, linc, &rhs, 0, res, inc);
    }
}


//  substract
namespace math
{
    template<typename T>
    void sub(size_t size, const T* lhs, size_t linc, const T* rhs, size_t rinc, T* res, size_t inc)
    {
        operate(std::minus<T>(), size, lhs, linc, rhs, rinc, res, inc);
    }

#ifdef __INTEL_MKL__
    template<>
    void sub(size_t size, const double* lhs, size_t linc, const double* rhs, size_t rinc, double* res, size_t inc)
    {
        vdSubI(size, lhs, linc, rhs, rinc, res, inc);
    }

    template<>
    void sub(size_t size, const float* lhs, size_t linc, const float* rhs, size_t rinc, float* res, size_t inc)
    {
        vsSubI(size, lhs, linc, rhs, rinc, res, inc);
    }
#endif

    template<typename T>
    void sub(size_t size, T lhs, const T* rhs, size_t rinc, T* res, size_t inc)
    {
        sub(size, &lhs, 0, rhs, rinc, res, inc);
    }

    template<typename T>
    void sub(size_t size, const T* lhs, size_t linc, T rhs, T* res, size_t inc)
    {
        sub(size, lhs, linc, &rhs, 0, res, inc);
    }
}

//  multiply
namespace math
{
    template<typename T>
    void mul(size_t size, const T* lhs, size_t linc, const T* rhs, size_t rinc, T* res, size_t inc)
    {
        operate(std::multiplies<T>(), size, lhs, linc, rhs, rinc, res, inc);
    }

#ifdef __INTEL_MKL__
    template<>
    void mul(size_t size, const double* lhs, size_t linc, const double* rhs, size_t rinc, double* res, size_t inc)
    {
        vdMulI(size, lhs, linc, rhs, rinc, res, inc);
    }

    template<>
    void mul(size_t size, const float* lhs, size_t linc, const float* rhs, size_t rinc, float* res, size_t inc)
    {
        vsMulI(size, lhs, linc, rhs, rinc, res, inc);
    }
#endif

    template<typename T>
    void mul(size_t size, T lhs, const T* rhs, size_t rinc, T* res, size_t inc)
    {
        mul(size, &lhs, 0, rhs, rinc, res, inc);
    }

    template<typename T>
    void mul(size_t size, const T* lhs, size_t linc, T rhs, T* res, size_t inc)
    {
        mul(size, lhs, linc, &rhs, 0, res, inc);
    }
}

//  divide
namespace math
{
    template<typename T>
    void div(size_t size, const T* lhs, size_t linc, const T* rhs, size_t rinc, T* res, size_t inc)
    {
        operate(std::divides<T>(), size, lhs, linc, rhs, rinc, res, inc);
    }

#ifdef __INTEL_MKL__
    template<>
    void div(size_t size, const double* lhs, size_t linc, const double* rhs, size_t rinc, double* res, size_t inc)
    {
        vdDivI(size, lhs, linc, rhs, rinc, res, inc);
    }

    template<>
    void div(size_t size, const float* lhs, size_t linc, const float* rhs, size_t rinc, float* res, size_t inc)
    {
        vsDivI(size, lhs, linc, rhs, rinc, res, inc);
    }
#endif

    template<typename T>
    void div(size_t size, T lhs, const T* rhs, size_t rinc, T* res, size_t inc)
    {
        div(size, &lhs, 0, rhs, rinc, res, inc);
    }

    template<typename T>
    void div(size_t size, const T* lhs, size_t linc, T rhs, T* res, size_t inc)
    {
        div(size, lhs, linc, &rhs, 0, res, inc);
    }
}

// power
namespace math
{
    template<typename T>
    void pow(size_t size, const T* lhs, size_t linc, const T* rhs, size_t rinc, T* res, size_t inc)
    {
        operate([](T base, T power) { return std::pow(base, power); }, size, lhs, linc, rhs, rinc, res, inc);
    }

#ifdef __INTEL_MKL__
    template<>
    void pow(size_t size, const double* lhs, size_t linc, const double* rhs, size_t rinc, double* res, size_t inc)
    {
        vdPowI(size, lhs, linc, rhs, rinc, res, inc);
    }

    template<>
    void pow(size_t size, const float* lhs, size_t linc, const float* rhs, size_t rinc, float* res, size_t inc)
    {
        vsPowI(size, lhs, linc, rhs, rinc, res, inc);
    }
#endif

    template<typename T>
    void pow(size_t size, T lhs, const T* rhs, size_t rinc, T* res, size_t inc)
    {
        pow(size, &lhs, 0, rhs, rinc, res, inc);
    }

    template<typename T>
    void pow(size_t size, const T* lhs, size_t linc, T rhs, T* res, size_t inc)
    {
        pow(size, lhs, linc, &rhs, 0, res, inc);
    }
}

//  exponential
namespace math
{
    template<typename T>
    void exp(size_t size, const T* operand, size_t oinc, T* res, size_t inc)
    {
        operate([](T exponent) { return std::exp(exponent); }, size, operand, oinc, res, inc);
    }

#ifdef __INTEL_MKL__
    template<>
    void exp(size_t size, const double* operand, size_t oinc, double* res, size_t inc)
    {
        vdExpI(size, operand, oinc, res, inc);
    }

    template<>
    void exp(size_t size, const float* operand, size_t oinc, float* res, size_t inc)
    {
        vsExpI(size, operand, oinc, res, inc);
    }
#endif
}

namespace math
{
    template<typename T>
    void scal(size_t size, T factor, T* operand, size_t oinc)
    {
        operate([factor](T value) { return factor * value; }, size, operand, oinc, operand, oinc);
    }

#ifdef __INTEL_MKL__
    template<>
    void scal(size_t size, double factor, double* operand, size_t inc)
    {
        cblas_dscal(size, factor, operand, inc);
    }

    template<>
    void scal(size_t size, float factor, float* operand, size_t inc)
    {
        cblas_sscal(size, factor, operand, inc);
    }
#endif
}

namespace math
{
    template<typename T>
    void axpy(size_t size, T factor, const T* lhs, size_t linc, T* rhs, size_t rinc)
    {
        operation([factor](T l, T r) { return l + factor * r; }, size, lhs, linc, rhs, rinc, rhs, rinc);
    }

#ifdef __INTEL_MKL__
    template<>
    void axpy(size_t size, double factor, const double* lhs, size_t linc, double* rhs, size_t rinc)
    {
        cblas_daxpy(size, factor, lhs, linc, rhs, rinc);
    }

    template<>
    void axpy(size_t size, float factor, const float* lhs, size_t linc, float* rhs, size_t rinc)
    {
        cblas_saxpy(size, factor, lhs, linc, rhs, rinc);
    }
#endif
}

namespace math
{
    template<typename T>
    void axpby(size_t size, T alpha, const T* lhs, size_t linc, T beta, T* rhs, size_t rinc)
    {
        operate([alpha, beta](T l, T r) { return alpha * l + beta * r; }, size, lhs, linc, rhs, rinc, rhs, rinc);
    }

#ifdef __INTEL_MKL__
    template<>
    void axpby(size_t size, double alpha, const double* lhs, size_t linc, double beta, double* rhs, size_t rinc)
    {
        cblas_daxpby(size, alpha, lhs, linc, beta, rhs, rinc);
    }

    template<>
    void axpby(size_t size, float alpha, const float* lhs, size_t linc, float beta, float* rhs, size_t rinc)
    {
        cblas_saxpby(size, alpha, lhs, linc, beta, rhs, rinc);
    }
#endif
}

namespace math
{
    template<typename T>
    T dot(size_t size, const T* lhs, size_t linc, const T* rhs, size_t rinc)
    {
        T result = 0;

        for (size_t i = 0; i < size; ++i)
        {
            result += lhs[i * linc] * rhs[i * rinc];
        }

        return result;
    }

#ifdef __INTEL_MKL__
    template<>
    double dot(size_t length, const double* lhs, size_t linc, const double* rhs, size_t rinc)
    {
        return cblas_ddot(length, lhs, linc, rhs, rinc);
    }

    template<>
    float dot(size_t length, const float* lhs, size_t linc, const float* rhs, size_t rinc)
    {
        return cblas_sdot(length, lhs, linc, rhs, rinc);
    }
#endif
}

/*
namespace math
{
    template<typename T>
    void gesv(size_t row, size_t column, const std::pair<const T*, size_t>& matrix, const std::pair<const T*, size_t> vector, std::pair<T*, size_t> results)
    {
    }

    template<typename T>
    void gesv(size_t scale, size_t batch, const std::pair<const double*, size_t>& matrix, const std::pair<const double*, size_t> vectors, std::pair<double*, size_t> results)
    {
        int pivots = new int[scale];

        dgesv(CblasRowMajor, scale, batch, matrix.first, lapack_int lda, pivots, vectors.first, lapack_int ldb);
        delete[] pivots;
    }

#ifdef __INTEL_MKL__
    template<>
    void gesv(size_t row, size_t column, const std::pair<const T*, size_t>& matrix, const std::pair<const T*, size_t> vector, std::pair<T*, size_t> results)
    {}
#endif
}
*/

//  distributions, keep consistant with standard library
namespace math::distribution
{
    template<typename T>
    class real
    {
    };

    template<typename T>
    class exponential
    {
    };

    template<typename T>
    class normal
    {};

    template<typename T>
    class lognormal
    {
    };
}
#endif //! _MATH_BLAS_