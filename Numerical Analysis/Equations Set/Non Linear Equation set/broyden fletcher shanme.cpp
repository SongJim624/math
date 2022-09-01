#include <string>
#include <mkl.h>

int broyden_fletcher_shanme(size_t length, const float * hessian, const float * secant, const float * difference, float * increment)
{
    float * temperory = (float *) mkl_malloc(length * length * sizeof(float), 64);

    //calculate the B_{k}y_{k}, result is stored in the variable increment
    cblas_sgemv(CblasRowMajor, CblasNoTrans, CblasNoTrans, length, length, 1, hessian, 1, difference, 1, 0, increment, 1);
    //calculating the \mu
    float factor = 1 + cblas_sdot(length, difference, 1, increment, 1) / cblas_dot(length, secant, 1, difference, 1);

    //calculate the denominator for the increment
    float denominator = 1 / cblas_sdot(length, secant, 1, difference, 1);

    //calculat the first item in the increment
    cblas_gemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, length, length, 1, factor, secant, denominator, secant, 1, 0, increment, 1);

    //calculate the second item in the increment
    cblas_gemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, length, length, 1, 1, secant, 1, difference, 1, 0, temperory, 1);
    cblas_gemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, length, length, length, -1, temperory, denominator, hessian, 1, 1, increment, 1);

    //calculate the third item in the increment
    cblas_gemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, length, length, 1, 1, difference, 1, secant, 1, 0, temperory, 1);
    cblas_gemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, length, length, length, -1, hessian, denominator, temperory, 1, 1, increment, 1);
};