#include "Matrix.h"

Matrix operator + (const Matrix& A, const Matrix& B)
{
    Matrix matrix(A.row, A.column);
    vsAdd(A.row * A.column, A.data, B.data, matrix.data);
    return matrix;
}

Matrix operator - (const Matrix& A, const Matrix& B)
{
    Matrix matrix(A.row, A.column);
    vsSub(A.row * A.column, A.data, B.data, matrix.data);
    return matrix;
}

Matrix operator * (const Matrix& A, const Matrix& B)
{
    Matrix matrix(A.row, B.column);

    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
        A.row, B.column, A.column, 
        1, A.data, A.column, B.data, B.column,
        0, matrix.data, matrix.column);
    return matrix;
}

Matrix operator * (const float& alpha, const Matrix& A)
{
    Matrix matrix(A.row, A.column);
    memcpy(matrix.data, A.data, A.row * A.column * sizeof(float));
    cblas_sscal(A.row * A.column, alpha, matrix.data, 1);
    return matrix;
}

Matrix& operator *= (Matrix& A, const float & alpha)
{
    cblas_sscal(A.row * A.column, alpha, A.data, 1);
    return A;
}

Matrix& operator += (Matrix& A, const Matrix& B)
{
    vsAdd(A.row * A.column, A.data, B.data, A.data);
    return A;
}

Matrix& operator -= (Matrix& A, const Matrix& B)
{
    vsSub(A.row * A.column, A.data, B.data, A.data);
    return A;
}

Matrix& operator *= (Matrix& A, const Matrix& B)
{
    float* data = new float[A.row * B.column]{ 0 };

    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
        A.row, B.column, A.column,
        1, A.data, A.column, B.data, B.column,
        0, data, B.column);

    A.column = B.column;
    delete[] A.data;
    A.data = data;

    return A;
}

bool operator == (const Matrix& A, const Matrix& B)
{
    if (A.row != B.row)
    {
        return false;
    }

    if (A.column != B.column)
    {
        return false;
    }

    float* temp = new float[A.row * A.column]{ 0 };
    vsSub(A.row * A.column, A.data, B.data, temp);

    return cblas_sasum(A.row * A.column, temp, 1) == 0;
}