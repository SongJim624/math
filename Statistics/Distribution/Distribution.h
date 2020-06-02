#include <vector>
#include <string>
#include "math.h"
#define PI 3.1415926

#ifndef _Distribution_
#define _Distribution_
template<typename T>
class Distribution
{
public:
    virtual std::vector<T> pdf(const std::vector<T> &X) = 0;
    virtual T pdf(const T &X) = 0;
    virtual std::vector<T> cdf(const std::vector<T> &X) = 0;    
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






