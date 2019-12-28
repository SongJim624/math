#pragma once
#include <time.h>
#include <vector>
#include <math.h>

template<typename T>
class PDF
{
public:
    virtual std::vector<T> pdf(const std::vector<T> &X) = 0;
};

template<typename T>
class CDF
{
public:
    virtual std::vector<T> cdf(const std::vector<T> &X) = 0;
};

template<class T>
class Sample
{
public:
    virtual std::vector<T> sample(const size_t& N) = 0;    
};

template<typename T>
T gamma(const T& x);