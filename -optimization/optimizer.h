#include <vector>
#include <memory>

#ifndef _SJML_Optimization_Optimizor_
#define _SJML_Optimization_Optimizor_
class Optimizer
{
public:
	class Objective;
	class Result;

public:
	virtual std::shared_ptr<Result> Optimize(Objective * objective) = 0;
	virtual ~Optimizer(){};
};

class Optimizer::Objective
{
public:
	virtual size_t scale() const = 0;
	virtual size_t dimension() const = 0;
	virtual size_t constraints() const = 0;

public:
	virtual const float* upper() const = 0;
	virtual const float* lower() const = 0;
	virtual const bool* integer() const = 0;

public:
	virtual void function(const float* decisions, float* objectives, float* voilations = nullptr) = 0;

public:
	virtual ~Objective() {}
};

class Optimizer::Result
{
public:
	virtual std::vector<std::vector<float>> decisions() const = 0;
	virtual std::vector<std::vector<float>> objectives() const = 0;

public:
	virtual void Write(const char* path) const = 0;

public:
	virtual ~Result() {};
};
#endif // !_SJML_Optimization_Optimizor_