#include "math.h"

template <class T>
class Interpolation
{
    size_t num, pos;
    T * x;
    T * y;

    size_t Search(const T &x)
    {
        if(x < this->x[0])
        {
            return 0;
        }
        else if(x > this->x[num - 1])
        {
            return num;
        }

        size_t left = 0, right = num - 1;
        size_t middle = num / 2;

        while((right - left) == 1)
        {
            if(x > this->x[middle])
            {
                left = middle
            }
            else if(x < this->x[middle])
            {
                right = middle;
            }
            else
            {
                return middle;
            }

            middle = (left + right) / 2;
        }  
    }
public:
    Interpolation(T * const &x, T* const &y, const size_t &num)
    : x(x), y(y), num(num), pos(0) {}

    virtual ~Interpolation() = 0;    
public:
    virtual void Interpolate(T* const &x, T* const &y, const size_t &L) = 0;
};

template<class T>
class Hermite
{
    T r, interval;
    T * dy;
public:
    Hermite(T * const &x, T* const &y, T * const &dy, const size_t &num)
    : r(0), interval(0), dy(dy), Interpolation(x, y, num){}
    ~Hermite()

public:
    Interpolate(T* const &x, T* const &y, const size_t &L)
    {
        for(size_t i = 0;i < L; ++i)
        {
            pos = Search(x[i]);

            if(pos == num|| pos == 0)
            {
                y[i] = Nan();
                printf("Warning : The point is not in the interpolation interval, please use the extrapolation !\n");
                printf("Warning : The value is set Nan !");
                continue;
            }

            r = (x[i] - this->x[pos]) / (this->x[pos + 1] - this->x[pos]);
            
            y[i] = this->y[pos] * (1 + 2 * r) * pow(r - 1, 2) +
                this->y[pos + 1] * (3 - 2 * r) * pow(r, 2) +
                (dy[pos] * r * pow(r - 1, 2) + 
                dy[pos + 1] *(r - 1) * pow(r, 2)) * interval;
        }

    }  
};

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