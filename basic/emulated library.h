#include <array>
#include <cmath>
#include <cstdlib>
#include <memory>
#include <algorithm>
#include <functional>
#include <numeric>

#ifndef _MATH_BLAS_
#define _MATH_BLAS_
namespace math
{
    template<typename T>
    T* allocate(size_t length)
    {
        return reinterpret_cast<T*>(std::calloc(length, sizeof(T)));
    }

    template<typename T>
    void free(T* pointer)
    {
        std::free(pointer);
    }

    template<typename T>
    void copy(size_t length, const T *source, size_t step, T *destination, size_t increment)
    {
        for (size_t i = 0; i < length; ++i)
        {
            *destination = *source;
            source += step;
            destination += increment;
        }
    }

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
namespace manth
{
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

    template<typename T>
    void xpby(size_t length, const T* left, size_t linc, T factor, T* right, size_t rinc)
    {
        operation(length, { left, right }, right, { linc, rinc, rinc }, [factor](T l, T r) { return l + factor * r; });
    }

    template<typename T>
    T dot(size_t length, const T* left, size_t linc, const T* right, size_t rinc)
    {
        auto temporary = std::unique_ptr<T[], decltype(&math::free<T>)>(math::allocate<T>(length), math::free<T>);
        mulI<T>(length, left, linc, right, rinc, temporary.get(), 1);
        return std::accumulate(temporary.get(), temporary.get() + length, 0.0);
    }
}
#endif //! _MATH_BLAS_