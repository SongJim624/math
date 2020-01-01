#include "stddef.h"

#ifndef _Vector_
#define _Vector_
class vector
{
private:
    long num;
    double* vec;
public:
    static dot(const vector& a, const vector& b);
    static cross(const vector& a, const vector& b);
    operator + (const vector& a);
    operator - (const vector& a);
    operator * (const double& a);
    operator / (const double& a);
    operator [](const long& index);
};
#endif
