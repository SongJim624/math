#ifdef __INTEL_MKL__
#include <mkl.h>
#endif

#include <array>
#include <cmath>
#include <cstdlib>
#include <memory>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>

#ifndef _MATH_BLAS_
#define _MATH_BLAS_
//  allocator of array
namespace math
{
#if defined __INTEL_MKL__
    template<typename T>
    using matrix = std::unique_ptr<T[], deltype(&mkl_free)>;

    template<typename T>
    matrix<T> allocate(size_t size)
    {
        return pointer<T>(mkl_calloc(size, sizeof(T), 64), mkl_free);
    }
#else
    template<typename T>
    using matrix = std::unique_ptr<T[], decltype(&std::free)>;

    template<typename T>
    matrix<T> allocate(size_t size)
    {
        return pointer<T>(mkl_calloc(size, sizeof(T), 64), mkl_free);
    }
#endif

    template<typename T, typename ...Args>
    void call(void* function, Args... args)
    {
        (*reinterpret_cast<T*>(function))(std::forward<Args>(args)...);
    }
};

//  copy fucntion
namespace math
{
    template<typename T, typename wrapper = std::pair<T*, size_t>>
    void copy(size_t length, const wrapper& source, wrapper destination)
    {
        for(size_t i = 0; i < length; ++i)
        {
            *(destination.first + i * destination.second) = *(source.first + i * source.second)
        }
    }

//#ifdef  __INTEL_MKL__
    template<>
    void copy<double>(size_t length, const std::pair<double, size_t>& source, std::pair<double, size_t> destination)
    {
        call<decltype(&mkl_dcopy)>(length, source.first, source.second, destination.first, destination.second);
    }

    template<>
    void copy<float>(size_t length, const T *source, size_t step, T *destination, size_t increment)
    {
        mkl_scopy(length, source, step, destination, increment);
    }
//#endif

    template<typename T>
    void operation(size_t length, std::array<const T*, 2> ptr, T * res, std::array<size_t, 3> inc, std::function<T(T, T)> opr)
    {
        for (size_t i = 0; i < length; ++i)
        {
            *res = opr(*ptr[0], ptr[1] == nullptr ? 0 : *ptr[1]);

            ptr[0] += inc[0];
            ptr[1] += inc[1];
            res += inc[2];
        }
    }
}

//  add
namespace math
{
    template<typename T>
    void add(size_t length, const std::pair<T*, size_t>& const std::pair<T*, size_t>&)


    template<typename T>
    void add(size_t length, const T *left, const T *right, T *results)
    {
        operation(length, { left, right }, results, { 1, 1, 1 }, [](T l, T r) {return l + r; });
    }

    template<typename T>
    void add(size_t length, const T* left, size_t linc, const T* right, size_t rinc, T* results, size_t inc)
    {
        operation(length, { left, right }, results, { linc, rinc, inc }, [](T l, T r) {return l + r; });
    }

#ifdef __INTEL_MKL__
    template<>
    void add<double>(size_t length, const T* left, const T* right, T* results)
    {
        vdAdd(length, left, right, results);
    }

    template<>
    void add<float>(size_t length, const T* left, const T* right, T* results)
    {
        vsAdd(length, left, right, results);
    }

    template<>
    void add<double>(size_t length, const T* left, size_t linc, const T* right, size_t rinc, T* results, size_t inc)
    {
        vdAddI(length, left, linc, right, rinc, results, inc);
    }

    template<>
    void add<float>(size_t length, const T* left, size_t linc, const T* right, size_t rinc, T* results, size_t inc)
    {
        vsAddI(length, left, linc, right, rinc, results, inc);
    }
#endif
}

//  substract
namespace math
{
    template<typename T>
    void sub(size_t length, const T *left, const T *right, T *results)
    {
        operation(length, { left, right }, results, { 1, 1, 1 }, [](T l, T r) {return l - r; });
    }

    template<typename T>
    void sub(size_t length, const T* left, size_t linc, const T* right, size_t rinc, T* results, size_t inc)
    {
        operation(length, { left, right }, results, { linc, rinc, inc }, [](T l, T r) {return l - r; });
    }
}

//  multiply
namespace math
{
    template<typename T>
    void mul(size_t length, const T *left, const T *right, T *results)
    {
        operation(length, { left, right }, results, { 1, 1, 1 }, [](T l, T r) {return l * r; });
    }

    template<typename T>
    void mul(size_t length, const T* left, size_t linc, const T* right, size_t rinc, T* results, size_t inc)
    {
        operation(length, { left, right }, results, { linc, rinc, inc }, [](T l, T r) {return l * r; });
    }
}

//  divide
namespace math
{
    template<typename T>
    void div(size_t length, const T *left, const T *right, T *results)
    {
        operation(length, { left, right }, results, { 1, 1, 1 }, [](T l, T r) {return l / r; });
    }

    template<typename T>
    void div(size_t length, const T* left, size_t linc, const T* right, size_t rinc, T* results, size_t inc)
    {
        operation(length, { left, right }, results, { linc, rinc, inc }, [](T l, T r) {return l / r; });
    }
}

// power
namespace math
{
    template<typename T>
    void pow(size_t length, const T* left, const T* right, T* results)
    {
        operation(length, { left, right }, results, { 1, 1, 1 }, [](T l, T r) {return std::pow(l, r); });
    }

    template<typename T>
    void pow(size_t length, const T* left, size_t linc, const T* right, size_t rinc, T* results, size_t inc)
    {
        operation(length, { left, right }, results, { linc, rinc, inc }, [](T l, T r) {return std::pow(l, r); });
    }
}

//  exponential
namespace math
{
    template<typename T>
    void exp(size_t length, const T* expontential, T* results)
    {
        operation(length, { expontential, nullptr }, results, { 1, 0, 1 }, [](T l, T r) {return std::exp(l); });
    }

    template<typename T>
    void exp(size_t length, const T* exponential, size_t linc, T* results, size_t inc)
    {
        operation(length, {exponential, nullptr }, results, { linc, 0, inc }, [](T l, T r) {return std::exp(l); });
    }
}

namespace math
{
    template<typename T>
    void scal(size_t length, T factor, T* vector, size_t inc)
    {
        operation(length, { vector, nullptr }, vector, { inc, 0, inc }, [factor](T l, T r) { return factor * l; });
    }

    template<>
    void scal<double>(size_t length, T factor, T* vector, size_t inc)
    {
        cblas_dscal(length, factor, vector, inc);
    }

    template<>
    void scal<float>(size_t length, T factor, T* vector, size_t inc)
    {
        cblas_sscal(length, factor, vector, inc);
    }
}

namespace math
{
    template<typename T>
    void xpby(size_t length, const T* left, size_t linc, T factor, T* right, size_t rinc)
    {
        operation(length, { left, right }, right, { linc, rinc, rinc }, [factor](T l, T r) { return l + factor * r; });
    }
}

namespace math
{
    template<typename T>
    T dot(size_t length, const T* left, size_t linc, const T* right, size_t rinc)
    {
        auto temporary = std::unique_ptr<T[], decltype(&math::free<T>)>(math::allocate<T>(length), math::free<T>);
        mulI<T>(length, left, linc, right, rinc, temporary.get(), 1);
        return std::accumulate(temporary.get(), temporary.get() + length, 0.0);
    }

#ifdef __INTEL_MKL__
    template<>
    double dot<double>(size_t length, const double* left, size_t linc, const double* right, size_t rinc)
    {
        return cblas_ddot(length, left, linc, right, rinc);
    }

    template<>
    float dot<float>(size_t length, const double* left, size_t linc, const double* right, size_t rinc)
    {
        return cblas_sdot(length, left, linc, right, rinc);
    }

#endif
}
#endif //! _MATH_BLAS_