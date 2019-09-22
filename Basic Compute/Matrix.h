#include "math.h"
#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
/*
The vector is redefined becaues the usage of
the std::vector would waste some of the memory
*/

enum Type {Row, Col};

class Vector
{
private:
    bool del;
    Type type;    
    double* vec;
    size_t num;

private:
    friend double dot(const Vector &a, const Vector& b);
    friend double norm(const Vector & vec);

public:
    Vector();
    Vector(const double* data, const size_t &num);
    Vector(const std::vector<double>& vec);

public:
    ~Vector();    
};

double dot(const Vector& a, const Vector& b);
double norm(const Vector& vec);

class Matrix
{
private:
    size_t num;    
    Vector* set;
private:
    void Transform(Type t);
    friend void Grams(Matrix &matrix);

private:

//Cosntructor
public:
    Matrix();
    Matrix(const Matrix& matrix);
    Matrix(const size_t &num);
    Matrix(const size_t &r, const size_t &c);
    Matrix(const double* data, const size_t &r, const size_t&c);

//Destructor
public:
    ~Matrix();

//Fucntion
public:    
    void size(size_t &r, size_t& c);
    void trans();
    void append(const Matrix& matrix);

//Operation
public:
    Matrix operator + (Matrix &matrix);
    Matrix operator - (Matrix &matrix);
    Matrix operator * (Matrix &matrix);
    Matrix operator / (Matrix &matrix);

public:
    double& operator () (const size_t &index);
    double& operator () (const size_t &r, const size_t&c); 
};

class Dense : public Matrix
{

};

class Band : public Matrix
{

};

class Sparse : public Matrix
{

};