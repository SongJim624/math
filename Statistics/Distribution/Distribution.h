#include <vector>
#include <string>
#include "math.h"
#define PI 3.1415926

#ifndef _Distribution_
#define _Disribution_
class Distribution
{
public:
    virtual std::vector<float> pdf(const std::vector<float> &X) = 0;
    virtual std::vector<float> cdf(const std::vector<float> &X) = 0;    
};
#endif

//extern "C" _declspec (dllexport) float _stdcall Beta();
//extern "C" _declspec (dllexport) Distribution* Normal();
//extern "C" _declspec (dllexport) Distribution* Beta();
//extern "C" _declspec (dllexport) _stdcall Distribution* Beta();
//extern "C" _declspec (dllexport) _stdcall Distribution* Beta();
//extern "C" _declspec (dllexport) _stdcall Distribution* Beta();
//extern "C" _declspec (dllexport) _stdcall Distribution* Beta();
//extern "C" _declspec (dllexport) _stdcall Distribution* Beta();
//extern "C" _declspec (dllexport) _stdcall Distribution* Beta();






