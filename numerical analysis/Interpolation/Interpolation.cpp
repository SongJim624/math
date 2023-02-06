#include "Interpolation.h"

template<typename T>
Interpolation<T>::Interpolation(T * X, T * Y, const long& length)
    : length(length), X(new T[length]), Y(new T[length])
{
    memcpy(this->X, X, length * sizeof(T));
    memcpy(this->Y, Y, length * sizeof(T));
}

template<typename T>
Interpolation<T>::~Interpolation()
{
    delete[] X;
    delete[] Y;
}

template<typename T>
void Interpolation<T>::Location(const T& x, long& left, long& right)
{
    left = 0;
	right = length - 1;

	while (right - left != 1)
	{
		long mid = (left + right) / 2;

		if (x == X[mid])
		{
            left = right = mid;
            return;
        }	

		x > X[mid] ? left = mid : right = mid;
	}
}