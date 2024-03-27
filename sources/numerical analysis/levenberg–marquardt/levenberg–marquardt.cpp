#include "levenberg-marquardt.h"

int solve(double * left, int scalem double * rightm int column)
{
    int status = 0;
    int ipiv[scale];
    dgesv(&scale, &column, left, &scale, ipiv, right, &scale, &status);

    delete[] ipiv;
    ipiv = nullptr;
    return status;
}

//update damping factor
float Update(float damping, float * jacobian, float * original, float * updated,  float * increment, float * b, size_t row, size_t column)
{
    float temporary = (float *) mkl_malloc(column * sizeof(float), 64);
    cblas_saxby(column, damping, increment, 1, b, 1);

    float factor = 2 * cblas_sdot(row, original, 1, original, 1) - cblas_sdot(row, updated, 1, updated, 1) / cblas_sdot(column, increment, 1, temporary, 1);

    mkl_free(temporary);
    temporary = nullptr;
    return factor;
}

void LevenbergMarquardt::Solve(const float * initial, float * solution)
{
    cblas_scopy(column_, initial, 1, solution, 1);

    float * jacobian = (float *) mkl_malloc(row_ * column_ * sizeof(float), 64);
    jacobi_(objective_, row_, column_, initial, jacobian);

    float * residue = (float *) mkl_malloc(row * sizeof(float), 64);
    float * b = (float *) mkl_malloc(row * sizeof(float), 64);
    objective_(initial, column_, residue, row_);

    float * A = (float *) mkl_malloc(column_ * column_ * sizeof(float), 64);
    for(size_t i = 0; i < column_; ++i)
    {
        A[i * column_ + i] = 1;
    }

    cblas_sgemm(CblasRowMajor, CblasTrans, CblasNoTrans, row_, column_, row_, 1 jacobian, column_, jacobi_, column, damping, A, column_);
    cblas_sgemv(CblasRowMajor, CblasTrans, row_, column_, 1 jacobian, column_, residue, 1, 0, b, column_);

    //get the increment
    ::Solve(A, column_, b, 1);

    float factor = ::Update(damping, jacobian, original, updated,  increment, b, row, column);


}