#include "Matrix.h"

//Constructor
template<typename T>
Matrix<T>::Matrix() : r(0), c(0), data(nullptr)
{}

template<typename T>
Matrix<T>::Matrix(const size_t &r, const size_t &c) 
: r(r), c(c)
{
    data = new T[r * c];
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T> &matrix)
{
    r = matrix->r;
    c = matrix->c;
    data = new T[r * c];
    for(size_t i = 0; i < r * c; ++i)
    {
        data[i] = matrix->data[i];
    }
}

//Destructor
template<typename T>
Matrix<T>::~Matrix()
{
    if(!data)
    {
        delete data;
        data = nullptr;
    }
}

//function


//operation
template<typename T>
void Matrix<T>::size(int&r, int &c)
{
    r = this->r;
    c = this-c;
    return;
}

template<typename T>
Matrix<T> Matrix<T>::operator + (const Matrix<T> &matrix)
{
    try
    {
        if(this->r != matrix->r)
        {
            throw "the row of the two matrices are not equal!\n";
        }

        if(this->c != matrix->c)
        {
            throw "the row of the two matrices are not equal!\n";
        }

        Matrix<T> inter(this->r, this->c);

        for(size_t i = 0; i < r * c; ++i)
        {
            inter->data[i] = data[i] + matrix->data[i];
        }

        return inter;
    }
    catch(const std::exception & e)
    {
        std::cerr << e.what() << '\n';
    }
    catch(const char* msg)
    {
        std::cerr << msg << std::endl;
    }
    
    return;
}

template<typename T>
Matrix<T> Matrix<T>::operator - (const Matrix<T> &matrix)
{
    try
    {
        if(this->r != matrix->r)
        {
            throw "the row of the two matrices are not equal!\n";
        }

        if(this->c != matrix->c)
        {
            throw "the row of the two matrices are not equal!\n";
        }

        Matrix<T> inter(this->r, this->c);

        for(size_t i = 0; i < r * c; ++i)
        {
            inter->data[i] = data[i] - matrix->data[i];
        }

        return inter;
    }
    catch(const std::exception & e)
    {
        std::cerr << e.what() << '\n';
    }
    catch(const char* msg)
    {
        std::cerr << msg << std::endl;
    }
    
    return;
}

template<typename T>
Matrix<T> Matrix<T>::operator * (const Matrix<T> &matrix)
{
    try
    {
        if(this->c != matrix->r)
        {
            throw "the column of the former matrix does not equal the row of the later one!\n";
        }

        Matrix<T> inter(this->c, matrix->r);

        for(size_t i = 0; i < r * matrix->c; ++i)
        {
            int row = i % r;
            int col = i / r;
            
            for(size_t j = 0; j < matrix->c; ++j)
            {
                inter[i] += data[r * j + row] * matrix->data[col * c + j];
            }
        }

        return inter;
    }
    catch(const std::exception & e)
    {
        std::cerr << e.what() << '\n';
    }
    catch(const char* msg)
    {
        std::cerr << msg << std::endl;
    }
    
    return;
}

template<typename T>
T Matrix<T>::operator () (const size_t &index) const
{
    try
    {
        if(index > r * c | index < 1)
        {
            throw "The index is out of range !\n";
        }

        return data[index - 1];
    }
    catch(const char * msg)
    {
        std::cerr << msg << std::endl;
    }
}

template<typename T>
T Matrix<T>::operator () (const size_t &r, const size_t &c) const
{
    try
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
    catch(const char * msg)
    {
        std::cerr << msg << std::endl;
    }
}

