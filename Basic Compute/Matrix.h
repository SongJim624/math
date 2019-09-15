#include "math.h"
#include <stdexcept>
#include <iostream>
#include <string>

template<typename T>
class Matrix
{
protected:
    size_t r, c;
    T * data;

//Cosntructor
public:
    Matrix();
    Matrix(const size_t &r, const size_t &c);
    Matrix(const Matrix<T>& matrix);

//Destructor
public:
    ~Matrix();

//Fucntion
public:    
    void size(int &r, int& c);
    void transposition();

//Operation
public:
    Matrix<T> operator + (const Matrix<T> &matrix);
    Matrix<T> operator - (const Matrix<T> &matrix);
    Matrix<T> operator * (const Matrix<T> &matrix);
    Matrix<T> operator / (const Matrix<T> &matrix);

    T operator () (const size_t &index) const;
    T operator () (const size_t &r, const size_t&c) const; 
};