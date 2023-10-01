#include <mkl.h>

namespace mkl
{
    void add();
//  level 1 functions, basic vector
    void addI(size_t length, const double* x, size_t xinc, const double* y, size_t yinc, double* r, size_t rinc);
    void subI(size_t length, const double* x, size_t xinc, const double* y, size_t yinc, double* r, size_t rinc);
    void mulI(size_t length, const double* x, size_t xinc, const double* y, size_t yinc, double* r, size_t rinc);
    void divI(size_t length, const double* x, size_t xinc, const double* y, size_t yinc, double* r, size_t rinc);
    void powI(size_t length, const double* x, size_t xinc, const double* y, size_t yinc, double* r, size_t rinc);
    void expI(size_t length, const double* x, size_t xinc, const double* y, size_t yinc, double* r, size_t rinc);
    void logI(size_t length, const double* x, size_t xinc, const double* y, size_t yinc, double* r, size_t rinc);

//
    void scal();
    double dot();
    void axpy();
    void axpby();
}