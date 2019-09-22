#include "Vector.h"

Vector::Vector()
: r(0), data(nullptr)
{}

Vector::Vector(std::vector<double> vec)
: r(vec.size()), data(new double[r])
{
    for(size_t i = 0; i < r; ++i)
    {
        *(data++) = vec[i];
    }
}

Vector::Vector(const size_t &r, const double* data)
: r(r), data(new double[r]) 
{
    if(data == nullptr)
    {
        return;
    }

    for(size_t i = 0; i < r; ++i)
    {
        this->data[i] = data[i];
    }
}

Vector::~Vector()
{
    if(data)
    {
        delete[] data;
    }
}

double dot(const Vector& a, const Vector& b)
{
    double sum = 0.0;

    for(size_t i = 0; i < a.r; ++i)
    {
        sum += a.data[i] * b.data[i];
    }

    return sum;
}