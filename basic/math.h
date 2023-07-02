#include <cstdlib>
#include <algorithm>

#ifndef _MATH_BASIC_ARTHEMIC_
#define _MATH_BASIC_ARTHEMIC_
namespace math
{
    /*
    template<typename T>
    concept Pointer = ()
    {};
    */

    double * allocate(size_t length);
    void free(double * vector);

    template<typename T>
    void add(size_t length, const T left, const T right, T results);


    void add(size_t length, const double * left, const double * right, double * results);
    void sub(size_t length, const double * left, const double * right, double * results);
    void div(size_t length, const double * left, const double * right, double * results);
    void pow(size_t length, const double * base, double power, double * results);
    void exp(size_t length, const double * left, const double * right, double * results);

    void copy(size_t length, const double* sources, size_t step, double *, size_t increment);
}
#endif //! _MATH_BASIC_ARTHEMIC_