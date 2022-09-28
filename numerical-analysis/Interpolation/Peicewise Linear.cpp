#include "Interpolation.h"

template<typename T>
Largrange<T>::Largrange(T * X, T * Y, const long& length)
	: Interpolation<T>(X, Y, length){}


template<typename T>
Largrange<T>::~Largrange(){}

template<typename T>
T Largrange<T>::Interpolate(const T& x)
{
	long left = 0, right = 0;
	
	Interpolation<T>::Location(x, left, right);

	return left == right ? Interpolation<T>::Y[left] :
		(Interpolation<T>::Y[left] * (Interpolation<T>::X[right] - x) + 
		 Interpolation<T>::Y[right] * (x - Interpolation<T>::X[left])) /
		(Interpolation<T>::X[right] - Interpolation<T>::X[left]);
}