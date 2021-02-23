#include "math.h"
#include <string>

#ifndef _INTERPOLATION_
#define _INTERPOLATION_
template<typename T>
class Interpolation
{
protected:
    const long length;
    T * X, * Y;

protected:
    void Location(const T& x, long& left, long& right);

public:
    Interpolation(T * X, T * Y, const long& length);
    virtual ~Interpolation();

    virtual T Interpolate(const T& x) = 0;
};

template<typename T>
class Largrange : public Interpolation<T>
{
public:
    Largrange(T * X, T * Y, const long& length);
    virtual ~Largrange();

    virtual T Interpolate(const T& x);
};

template<class T>
class Hermite : public Interpolation<T>
{
private:    
    T * dY;

private:
    T H0(const T& rel);
    T H1(const T& rel);

public:
    Hermite(T * X, T * Y, T * dY, const long& length);
    virtual ~Hermite();

    virtual T Interpolate(const T& x);
};

template<typename T>
class Spline : public Interpolation<T>
{
private:
    Hermite<T>* hermite;

public:
    Spline(T* X, T* Y, const long& length, size_t type);
    ~Spline();

    virtual T Interpolate(const T& x);
};

/*
template <class T>
class Spline : public Hermite
{
public:
    Spline(T * const &x, T* const &y, T * const &dy, const size_t &num)
    : Hermite(x, y, nullptr, num){ dy = new T[num];}

    void Boundary(const T &upper = 0, const T &lower = 0, const size_t &type = 2)
    {
        T *a = new T [num];
        T *b = new T [num];
        T *c = new T [num];
        T *f = new T [num];

        switch(type)
        {
        case 1:
        {

        }
        case 2:
        {
            b[0] = 1, c[0] = 0; f[0] = upper;
            b[num - 1] = 1; a[num - 1] = 0; f[num - 1] = lower;
            break;
        }
        case default{printf("The boundary condition type is not supported !\n"); abort();}
        }

        for(size_t i = 1; i < num - 1; ++i)
        {
            a[i] = (x[i] - x[i - 1]) / (x[i + 1] - x[i - 1]);
            b[i] = 2.0;
            c[i] = 1 - a[i];
            f[i] = 6 * ((y[i + 1] - y[i]) / (x[i + 1] - x[i]) -
                (y[i] - y[i - 1]) / (x[i] - x[i - 1])) / (x[i + 1] - x[i - 1]);  
        }

        T *x= new T[num];

        Chasing(a, b, c, f, num, x);

        for(size_t i = 0; i < num - 1; ++i)
        {
            dy[i] = -(this->x[i + 1] - this->x[i]) * 
            (x[i] / 3 + x[i + 1] / 6) -
            (y[i + 1] - y[i]) / (this->x[i + 1] - this->x[i]);
        }

        dy[num - 1] = (this->x[num - 1] - this->x[num - 2]) * 
            (x[num - 2] / 6 + x[num - 1] / 3) + 
            (y[num - 1] - y[num - 2]) / (this->x[num - 1] - this->x[num - 2]);

        delete[] a;
        delete[] b;
        delete[] c;
        delete[] f;
        delete[] x;
    }

    ~Spline()
    {
        delete[] dy;
    }
}
*/
#endif 