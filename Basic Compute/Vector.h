#include "stddef.h"
#include <vector>
#include <iostream>

class Vector
{
private:
    friend double dot(const Vector& a, const Vector& b);

protected:
    size_t r;
    double* data;

//Constructor
public:
    Vector();
    Vector(std::vector<double> vec);
    Vector(const size_t &r, const double* data = nullptr);

public:
    ~Vector();
};