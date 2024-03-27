#include "broyden.h"

void broyden_fletcher_shanme(size_t scale, float * hessian, const float * secant, float * original, const float * updated)
{
    float * temperory = (float *) mkl_malloc(scale * scale * sizeof(float), 64);
    float * increment = (float *) mkl_malloc(scale * scale * sizeof(float), 64);
    float * difference = (float *) mkl_malloc(scale * sizeof(float), 64);

    //calculate the difference between the updated and original function value
    vsSub(scale, updated, original, difference);

    //calculate the denominator for the increment
    float denominator = 1 / cblas_sdot(scale, secant, 1, difference, 1);

    //calculate the B_{k}y_{k}, result is stored in the temporary array
    cblas_sgemv(CblasRowMajor, CblasNoTrans, scale, scale, 1, hessian, scale, difference, 1, 0, temperory, 1);
    //calculating the \mu
    float factor = 1 + cblas_sdot(scale, difference, 1, temperory, 1) * denominator;

    //calculat the first item in the increment, because the original Hessian is not needed in this term
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, scale, scale, 1, factor * denominator, secant, 1, secant, scale, 0, increment, scale);

    //calculate the second item in the increment
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, scale, scale, 1, 1, secant, 1, difference, scale, 0, temperory, scale);
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, scale, scale, scale, -denominator, temperory, scale, hessian, scale, 1, increment, scale);

    //calculate the third item in the increment
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, scale, scale, 1, 1, difference, 1, secant, scale, 0, temperory, scale);
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, scale, scale, scale, -denominator, hessian, scale, temperory, scale, 1, increment, scale);

    //update the Hessian and function value
    vsAdd(scale * scale, hessian, increment, hessian);
    //might be replaced with swap
    cblas_scopy(scale, updated, 1, original, 1);

    mkl_free(temperory);
    mkl_free(increment);
    mkl_free(difference);
};

void broyden_rank_1(size_t scale, float * hessian, const float * secant, float * original, const float * updated)
{
    float * increment = (float *) mkl_malloc(scale * scale * sizeof(float), 64);
    float * difference = (float *) mkl_malloc(scale * sizeof(float), 64);

    //calculate the difference between the updated and original function value
    vsSub(scale, updated, original, difference);

    //calculate the denominator, B_{k}y_{k}, result is stored in the temporary array
    cblas_sgemv(CblasRowMajor, CblasNoTrans, scale, scale, 1, hessian, 1, difference, 1, 0, increment, 1);
    float denominator = cblas_sdot(scale, secant, 1, increment, 1);

    //
    vsSub(scale, secant, increment, increment);

    //calculat the first item in the increment, because the original Hessian is not needed in this term
//    cblas_sgemm(CblasRowMajor, CblasTrans, CblasNoTrans, scale, scale, 1, 1, secant, increment, secant, 1, 0, increment, 1);
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, scale, scale, 1, 1, increment, 1, secant, 1, 0, increment, 1);
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, scale, scale, scale, -1, increment, denominator, hessian, 1, 1, increment, 1);

    //update the Hessian and function value
    vsAdd(scale * scale, hessian, increment, hessian);
    cblas_scopy(scale, updated, 1, original, 1);

    mkl_free(increment);
    mkl_free(difference);
}

void Broyden::Updator(size_t code)
{
    switch (code)
    {
    case 0:
    {
        updator_ = [](size_t scale, float* hessian, const float* secant, float* original, const float* updated)
        {
            broyden_fletcher_shanme(scale, hessian, secant, original, updated);
        };
        return;
    }
    }
}