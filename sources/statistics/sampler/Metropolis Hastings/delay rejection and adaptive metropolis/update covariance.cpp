#include <mkl.h>
#include <vector>

void Update(const float * samples, size_t row, size_t column, float * covariance, float* means, float& weight)
{
    double * temporary = (doule *) mmkl_calloc(column * (column + 1), sizeof(double), 64);

    for(size_t r = 0; r < row; ++r)
    {
        float * values = temporary + column + column;
        vsSub(column, samples + r * column, means, values);

//update of the mean
        cblas_daxpy(column, 1.0 / (1.0 + weight), values, 1, means, 1);

//update of the covariance
        cblas_dgemm (CblasRowMajor, CblasTrans, CblasNoTrans, 1, column, column, 1, values, 1, values, 1, 0, temporary, 1);
        cblas_daxpby(column * column, 1.0 / weight, temporary, 1, 1.0 - 1.0 / weight, covariance, 1);

        weight++;
    }

    mkl_free(temporary);
    temporary = nullptr;
}