#include "Matrix.h"

double norm(double *a,  const size_t &num)
{
    double n = 0.0;

    size_t i = 0;
    while(i < num)
    {
        n += pow(*(a++), 2);
    }

    return sqrt(n);
}

double dot(double *a, double *b, const size_t &num)
{
    double sum = 0.0;
    size_t i = 0;
    while(i < num)
    {
        sum += *(a++) * *(b++);
        ++i;
    }

    return  sum;
}


void Grams(Matrix& A)
{
    size_t c = A.c;
    size_t r = A.r;
    A.Transform(Column);

    double ** B = new double*[c];
    for(size_t i = 0; i < c; ++i)
    {
        B[i] = new double[r];
    }

    double ** T = new double* [c];
    for(size_t i = 0; i < c; ++i)
    {
        T[i] = new double [c];
    }


    double t = 0;

    double *x, *b;
    double *inter = new double [c];

    for(size_t j = 0; j < c; ++j)
    {
        x = A.data + j * r;

        for(size_t i = 0; i < j; ++i)
        {
            b = B[i];
            T[j][i] += dot(x, b, c);
        }

        for(size_t i = 0; i < j; ++i)
        {
           
        }

    }
}