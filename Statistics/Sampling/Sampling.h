#pragma once
#include <time.h>
#include <vector>
#include <math.h>
#include "Distribution.h"

template<class T>
class Sample
{
public:
    virtual std::vector<T> sample(const dist& dist, const size_t& N) = 0;    
};

template<typename T>
T gamma(const T& x);