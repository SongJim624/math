#include "math.h"

template <class T>
{
private:
    T* vector;
    bool Construct;
public:
    Vector(const size_t & n)
    : vector = new T[n], Construct(true)
    {};


    ~Vector()
    {
        if(Construct)
        {
            delete[] vector;
        }
    }
}

template <class T>
class Matrix
{
private:
    T *matrix;

public:
    Matrix(const size_t &M,const size_t &N)
    {
        matrix = new T[M * N];
    }

    T& operator [][](const size_t &i, const size_t &j)
    {
        return matrix[i * M + j - 1];
    }
};