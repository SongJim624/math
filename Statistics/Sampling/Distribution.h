#include <vector>
#include "math.h"

#ifndef _Distribution_
#define _Disribution_
template<typename T>
class dist
{
public:
    virtual std::vector<T> pdf(const std::vector<T> &X) = 0;
    virtual std::vector<T> cdf(const std::vector<T> &X) = 0;    
};
#endif
