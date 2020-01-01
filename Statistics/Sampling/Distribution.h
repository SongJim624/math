#include <vector>
#include "math.h"

#ifndef _Distribution_
#define _Disribution_
class Distribution
{
public:
    virtual std::vector<float> pdf(const std::vector<float> &X) = 0;
    virtual std::vector<float> cdf(const std::vector<float> &X) = 0;    
};
#endif
