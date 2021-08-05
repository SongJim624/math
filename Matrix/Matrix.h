#include <list>
#include <map>
#include <array>
#include <vector>
#include <algorithm>
#include <mkl.h>

#ifndef _IMATRIX_
#define _IMATRIX_
//This interface is to promise that the class of all types of matrix have some basic functions
class IMatrix
{
private:
    friend bool operator == (const IMatrix& A, const IMatrix& B);

protected:
    long row, column;

public:
    virtual float& operator () (long r, long c) = 0;
    virtual float operator() (long r, long c) const = 0;

public:
    long Row() const;
    long Column() const;

public:
    IMatrix(const long& r, const long& c);
    ~IMatrix();
};
#endif //! _IMATRIX_

//default type of Matrix is the dense matrix 
#ifndef __MATRIX__
#define __MATRIX__
class Matrix : public IMatrix
{
protected:
    float* data;

protected:

    friend Matrix& Transposite(Matrix& A);

    friend Matrix operator * (const Matrix& A, const Matrix& B);
    friend Matrix operator * (const float& alpha, const Matrix& A);
    friend Matrix operator + (const Matrix& A, const Matrix& B);
    friend Matrix operator - (const Matrix& A, const Matrix& B);

    friend Matrix& operator += (Matrix& A, const Matrix& B);
    friend Matrix& operator -= (Matrix& A, const Matrix& B);
    friend Matrix& operator *= (Matrix& A, const Matrix& B);
    friend Matrix& operator *= (Matrix& A, const float& alpha);
    
    friend bool operator == (const Matrix& A, const Matrix& B);
    
public:
    virtual float& operator () (long r, long c);
    virtual float operator() (long r, long c) const;
    Matrix& operator = (const Matrix& B);

public:
    Matrix();
    Matrix(const Matrix& matrix);
    Matrix(long r, long c);
    Matrix(long row, long column, float * data);
    Matrix(long row, long column,  float ** data);
    Matrix(const std::vector<std::vector<float>>& matrix);
    virtual ~Matrix();
};


Matrix & Transposite(Matrix& M);
//Matrix & Inverse(Matrix& M);
#endif

#ifndef _SPARSE_
#define _SPARSE_
//This is of the CSR format
class Sparse : public IMatrix
{
private:
    std::map<long, std::map<long, float>> data;

    friend Sparse& operator += (Sparse& A, const Sparse& B);
    friend Sparse& operator -= (Sparse& A, const Sparse& B);
    friend IMatrix& operator += (IMatrix& A, const Sparse& B);
    friend IMatrix& operator -= (IMatrix& A, const Sparse& B);

public:
    virtual float& operator ()(long r, long c);
    virtual float operator () (long r, long c) const;
public:
    Sparse(const long& row, const long& column);
};
#endif //! _SPARSE_

#ifndef _BAND_
#define _BAND_

#endif //! _BAND_

#ifndef _OPERATORS_
#define _OPERATIRS_
//operators for the dense and sparse hybrid
bool operator == (const IMatrix& A, const IMatrix& B);
IMatrix& operator += (IMatrix& A, const Sparse& B);
IMatrix& operator -= (IMatrix& A, const Sparse& B);
IMatrix& Transposite(IMatrix& A);

//Operators for the dense matrix
Matrix operator + (const Matrix& A, const Matrix& B);
Matrix operator - (const Matrix& A, const Matrix& B);
Matrix operator * (const Matrix& A, const Matrix& B);
Matrix operator * (const float& alpha, const Matrix& A);

Matrix& operator += (Matrix& A, const Matrix& B);
Matrix& operator -= (Matrix& A, const Matrix& B);
Matrix& operator *= (Matrix& A, const Matrix& B);
Matrix& operator *= (Matrix& A, const float& alpha);

bool operator == (const Matrix& A, const Matrix& B);
Matrix& Tranposite(Matrix& A);

Sparse& operator += (Sparse& A, const Sparse& B);
Sparse& operator -= (Sparse& A, const Sparse& B);
Sparse& Transposite(Sparse& A);
#endif //! _OPERATORS_