#include "../Statistics/Sampling.h"
#include <vector>
#include <array>

#ifndef _Sensitivity_
#define _Sensitiviry_

typedef std::vector<float> (*Function)(const std::vector<float>&)

class SA
{
public:
    virtual std::vector<float> analysis(Function func, std::vector<Distribution*>& parameter) = 0;
};
#endif