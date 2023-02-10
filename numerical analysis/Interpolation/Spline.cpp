#include "Interpolation.h"

template<typename T>
Spline::Spline(T* X, T* Y, const long& length, size_t type)
	: Interpolation<T>(X, Y, length), hermite(nullptr)
{
	T* dY = new T[length];


    T* a = new T[num];
    T* b = new T[num];
    T* c = new T[num];
    T* f = new T[num];

    switch (type)
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
    }

    for (long i = 1; i < length - 1; ++i)
    {
        a[i] = (X[i] - X[i - 1]) / (X[i + 1] - X[i - 1]);
        b[i] = 2.0;
        c[i] = 1 - a[i];

        f[i] = 6 * ((Y[i + 1] - Y[i]) / (X[i + 1] - X[i]) -
            (Y[i] - Y[i - 1]) / (X[i] - X[i - 1])) / (X[i + 1] - X[i - 1]);
    }



	hermite = new Hermite(X, Y, dY, length);


}

template<typename T>
Spline<T>::~Spline()
{
	delete hermiet;
}

template<typename T>
T Spline<T>::Interpolate(const T& x)
{
	return hermite->Interpolate(x);
}