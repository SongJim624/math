#include "Vector.h"

enum Type{ Dense, Sparse, Diag};

#ifndef _Matrix_
#define _Matrix_
template<typename T>
class Matrix
{
protected:  
    Type type;
    unsigned long row, col;

public:
    virtual Matrix<T> operator + (const Matrix<T>& a);
    virtual Matrix<T> operator - (const Matrix<T>& a);
    virtual Matrix<T> operator * (const Matrix<T>& a);
    virtual Matrix<T> operator * (const Vector<T>& a);

public:
    virtual void Trans();
};
#endif

template<typename T>
class Dense : public Matrix
{
private:    
    T* data;

private:
    virtual Matrix<T> operator +(const Matrix<T>& a);

public:
    Dense();    
};

/*
The sparse matrix is stored by the HYB (EII+COO) method
*/
template<typename T>
class Sparse : public Matrix
{};

/*
This is for the convenience of the usage
of L, U, Diag, Tridiag, band involvng the 
DIA method
*/
template<typename T>
class Diag : public Matrix
{

};
