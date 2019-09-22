#include "Matrix.h"

//Constructor
Matrix::Matrix() : r(0), c(0), data(nullptr)
{}

Matrix::Matrix(const size_t &num)
: r(num), c(1), data(new double [num])
{}

Matrix::Matrix(const size_t &r, const size_t &c) 
: r(r), c(c), data(new double[r * c])
{}

Matrix::Matrix(const Matrix &matrix)
: r(matrix.r), c(matrix.c), data(new double[r * c])
{
    for(size_t i = 0; i < r * c; ++i)
    {
        data[i] = matrix.data[i];
    }
}

Matrix::Matrix(const double* data, const size_t &r, const size_t&c)
:r(r), c(c), data(new  double[r * c])
{
    for(size_t i = 0; i < r * c;++i)
    {
        this->data[i] = data[i];
    }
}

//Destructor
Matrix::~Matrix()
{
    if(!data)
    {
        delete[] data;
        data = nullptr;
    }
}

//function
void Matrix::Transform(Type t)
{
    if(type == t)
    {
        return;
    }

    double * inter = new double[num];
    switch(type)
    {
    case Row:
    {        
        for(size_t i = 0; i < num; ++i)
        {
            inter[(i % c) * r + (i / c)] = data[i];
        }

        break;
    }
    case Column:
    {
        for(size_t i = 0; i < num; ++i)
        {
            inter[(i % r) * c + (i / r)] = data[i];
        }

        break;
    }
    }

    delete[] data;
    data =  inter;
    type = t;
    return;
}

void Matrix::size(size_t&r, size_t &c)
{
    r = this->r;
    c = this->c;
    return;
}

void Matrix::append(const Matrix& matrix)
{
    if(r != matrix.r)
    {
        throw "The rows of the two matrices do not equal!\n";
    }
    
    size_t c_temp = c + matrix.c;
    double* temp = new double[r * c_temp];
    
    size_t i = 0;
    for(; i < r * c; ++i)
    {
        temp[i] = data[i];
    }
    
    size_t j = i;
    for(;i < r * c_temp; ++i)
    {
        temp[i] = matrix.data[i - j];
    }
    
    c = c_temp;
    delete[] data;
    data = temp;
}

void Matrix::trans()
{
    double* inter = new double[r * c];
    
    for(size_t i = 0; i < r * c; ++i)
    {
        inter[i] = data[(i % c) * r + (i / c)];
    }
    
    delete[] data;
    data = inter;

    size_t in = c;
    c = r;
    r = in;
}

//Operation
Matrix Matrix::operator + (Matrix &matrix)
{
    if(this->r != matrix.r)
    {
        throw "the row of the two matrices are not equal!\n";
    }

    if(this->c != matrix.c)
    {
        throw "the row of the two matrices are not equal!\n";
    }

    Matrix inter(this->r, this->c);

    for(size_t i = 0; i < r * c; ++i)
    {
        inter.data[i] = data[i] + matrix.data[i];
    }

    return inter;
}


Matrix Matrix::operator - (Matrix &matrix)
{
    if(this->r != matrix.r)
    {
        throw "the row of the two matrices are not equal!\n";
    }

    if(this->c != matrix.c)
    {
        throw "the row of the two matrices are not equal!\n";
    }

    Matrix inter(this->r, this->c);

    for(size_t i = 0; i < r * c; ++i)
    {
        inter.data[i] = data[i] - matrix.data[i];
    }

    return inter;
}


Matrix Matrix::operator * (Matrix &matrix)
{
    if(this->c != matrix.r)
    {
        throw "the column of the former matrix does not equal the row of the later one!\n";
    }

    Matrix inter(this->c, matrix.r);
    this->Transform(Row);
    matrix.Transform(Column);


    for(size_t i = 0; i < r * matrix.c; ++i)
    {
        unsigned long row = i % r;
        unsigned long col = i / r;
            
        for(size_t j = 0; j < matrix.c; ++j)
        {
            inter.data[i] += data[r * j + row] * matrix.data[col * c + j];
        }
    }

    return inter;
}

double& Matrix::operator () (const size_t &index)
{
    if(index > r * c | index < 1)
    {
        throw "The index is out of range !\n";
    }

    return data[index - 1];
}


double& Matrix::operator () (const size_t &r, const size_t &c)
{
    if(r > this->r | r < 1)
    {
        throw "The row is out of range !\n";
    }

    if(c > this->c | c < 1)
    {
        throw "The column is out of range !\n";
    }

    return data[this->r * (c - 1) + r];
}
