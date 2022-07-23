//#include "Continuous/Headers.h"
//#include "Discrete/Headers.h"
#include <vector>
#include <cassert>

class Matrix;

class Distribution
{
protected:
    virtual void density(float *, float *, size_t);
    virtual float probability(float *, float *, size_t);

public:
    template<typename T>
    T density(const T& values)
    {
        if(typeid(T) == typeid(float))
        {
            float result = 0;

            return result;
        }


        assert("type not supported!");
    }

    template<typename T>
    T probabilitt(const T& values)
    {

    }
};