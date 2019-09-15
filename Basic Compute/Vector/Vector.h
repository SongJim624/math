#include "stddef.h"

#ifndef _Vector_
#define _Vector_
template<typename T>
class Vector
{
    long num;
    T* vec;
public:
    static Vector<T> dot(const Vector<T>& a, const Vector<T>& b);
    static Vector<T> cross(const Vector<T>& a, const Vector<T>& b);
    Vector<T> operator + (const Vector<T>& a);
    Vector<T> operator - (const Vector<T>& a);
    Vector<T> operator * (const T& a);
    Vector<T> operator / (const T& a);
};
#endif