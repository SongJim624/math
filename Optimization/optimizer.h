#include <vector>

#ifndef _SJML_Optimization_Optimizor_
#define _SJML_Optimization_Optimizor_

class Information
{
public:
	virtual size_t decision() const = 0;
	virtual size_t objective() const = 0;

public:
	virtual const float* upper() const = 0;
	virtual const float* lower() const = 0;
	virtual const bool* integer() const = 0;

public:
	virtual void function(const float * decisions,  float * objectives) = 0;
//constraints further considered
//	virtual void function(const float * decisions,  float * objectives) = 0;
};

class Configuration
{
};

class Optimizer
{
public:
	virtual std::vector<std::vector<float>> Optimize(Information *) = 0;
	virtual ~Optimizer(){};
};
#endif // !_SJML_Optimization_Optimizor_