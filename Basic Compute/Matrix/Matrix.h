#include "Vector.h"

#ifndef _Matrix_
#define _Matrix_
template<typename T>
Matrix
{
private:    
    long row, col;
    T* data;
public:
    Matrix<T> operator + (const Matrix<T>& a);
    Matrix<T> operator - (const Matrix<T>& a);
    Matrix<T> operator * (const Matrix<T>& a);
    Matrix<T> operator * (const Vector<T>& a);
    void Trans();
};
#endif

template<typename T>
Matrix<T> Inverse(Matrix<T>& matrix);
