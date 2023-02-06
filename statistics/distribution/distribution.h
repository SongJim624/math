//#include "Continuous/Headers.h"
//#include "Discrete/Headers.h"
#include <vector>
#include <cassert>
#include <mkl.h>

class Matrix;

class Distribution
{
public:
    virtual void density(size_t length, const float * locations, float * values) = 0;
    virtual float probability(size_t length, const float * locations, float * values) = 0;

public:
    virtual ~Distribution(){};
};