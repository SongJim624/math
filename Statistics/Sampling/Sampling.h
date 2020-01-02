#pragma once
#include <time.h>
#include "../Distribution/Distribution.h"
#define Precision 99999999

float Rand();

class Sampling
{
public:
    virtual std::vector<float> sampling(const size_t& N, Distribution* dist = nullptr) = 0;
    virtual void ChangeParameter(const float& a = 0.0, const float& b = 0.0,
        const float& c = 0.0, const float& d = 0.0) = 0;
};

_declspec (dllexport) Sampling* _stdcall Box_Muller(const float&mu = 0.0, const float& sigma = 1.0, const size_t& type = 1);
_declspec (dllexport) Sampling* _stdcall Metropolis_Hasting(const float& X0);