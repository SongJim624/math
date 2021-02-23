#include "Interpolation.h"

template<typename T>
T Hermite<T>::H0(const T& rel)
{
    return (1.0 + 2.0 * abs(rel)) * pow(1 - abs(rel), 2);
}

template<typename T>
T Hermite<T>::H1(const T& rel)
{
    return rel * pow(1 - abs(rel), 2);
}

template<typename T>
Hermite<T>::Hermite(T * X, T * Y, T * dY, const long& length)
    : Interpolation<T>(X, Y, length), dY(new T [length])
{
    std::memcpy(this->dY, dY, length * sizeof(T));
}

template<typename T>
Hermite<T>::~Hermite()
{
    delete[] dY;
}

template<typename T>
T Hermite<T>::Interpolate(const T& x)
{
    long left = 0, right = 0;
    Interpolation<T>::Location(x, left, right);

    T l = (x - Interpolation<T>::X[left]) / (Interpolation<T>::X[right] - Interpolation<T>::X[left]);
    T r = (x - Interpolation<T>::X[right]) / (Interpolation<T>::X[right] - Interpolation<T>::X[left]);
 
    return left == right ? Interpolation<T>::Y[left] :
            Interpolation<T>::Y[left] * H0(l) + Interpolation<T>::Y[right] * H0(r) + 
            (Interpolation<T>::X[right] - Interpolation<T>::X[left]) *        
            (dY[left] * H1(l) + dY[right] * H1(r));
}
