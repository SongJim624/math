#include <string>
#include <mkl.h>

void broyden_fletcher_shanme(size_t scale, float * hessian, const float * secant, float * original, const float * updated)
{
    float * temperory = (float *) mkl_malloc(scale * scale * sizeof(float), 64);
    float * inrement = (float *) mkl_malloc(scale * scale * sizeof(float), 64);
    float * difference = (float *) mkl_malloc(scale * sizeof(float), 64);

    //calculate the difference between the updated and original function value
    vsSub(scale, updated, original, difference);

    //calculate the denominator for the increment
    float denominator = 1 / cblas_sdot(scale, secant, 1, difference, 1);

    //calculate the B_{k}y_{k}, result is stored in the temporary array
    cblas_sgemv(CblasRowMajor, CblasNoTrans, CblasNoTrans, scale, scale, 1, hessian, 1, difference, 1, 0, temperory, 1);
    //calculating the \mu
    float factor = 1 + cblas_sdot(scale, difference, 1, temperory, 1) / denominator;

    //calculat the first item in the increment, because the original Hessian is not needed in this term
    cblas_gemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, scale, scale, 1, factor, secant, denominator, secant, 1, 0, increment, 1);

    //calculate the second item in the increment
    cblas_gemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, scale, scale, 1, 1, secant, 1, difference, 1, 0, temperory, 1);
    cblas_gemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, scale, scale, scale, -1, temperory, denominator, hessian, 1, 1, increment, 1);

    //calculate the third item in the increment
    cblas_gemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, scale, scale, 1, 1, difference, 1, secant, 1, 0, temperory, 1);
    cblas_gemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, scale, scale, scale, -1, hessian, denominator, temperory, 1, 1, increment, 1);

    //update the Hessian and function value
    vsAdd(scale * scale, hessian, increment, hessian);
    cblas_sscopy(scale, updated, 1, origianl, 1);

    mkl_free(temperory);
    mkl_free(increment);
    mkl_free(difference);
};

void broyden_rank_1(size_t scale, float * hessian, const float * secant, float * original, const float * updated)
{
    float * inrement = (float *) mkl_malloc(scale * scale * sizeof(float), 64);
    float * difference = (float *) mkl_malloc(scale * sizeof(float), 64);

    //calculate the difference between the updated and original function value
    vsSub(scale, updated, original, difference);

    //calculate the denominator, B_{k}y_{k}, result is stored in the temporary array
    cblas_sgemv(CblasRowMajor, CblasNoTrans, CblasNoTrans, length, length, 1, hessian, 1, difference, 1, 0, increment, 1);
    float denominator = cblas_sdot(scale, scecant, 1, increment, 1);

    //
    vsSub(scale, secant, increment, increment);

    //calculat the first item in the increment, because the original Hessian is not needed in this term
    cblas_gemm(CblasRowMajor, CblasTrans, CblasNoTrans, scale, scale, 1, 1, secant, increment, secant, 1, 0, increment, 1);
    cblas_gemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, scale, scale, 1, 1, increment, 1, secant, 1, 0, increment, 1);
    cblas_gemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, scale, scale, scale, -1, increment, denominator, hessian, 1, 1, increment, 1);

    //update the Hessian and function value
    vsAdd(scale * scale, hessian, increment, hessian);
    cblas_sscopy(scale, updated, 1, origianl, 1);

    mkl_free(temperory);
    mkl_free(increment);
    mkl_free(difference);
}