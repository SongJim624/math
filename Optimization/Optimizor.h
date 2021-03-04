#ifndef _Mathematical_Tools_Optimization_Optimizor_
#define _Mathematical_Tools_Optimization_Optimizor_
class Function
{
public:
	virtual void function(float* decisions, const long& decision_size, float* objectives, const long& objective_size) = 0;
};

class Constraints
{
public:
	virtual float constraints(float* decisions, const long& decision_size, float* objectives, const long& objective_size) = 0;
};

class Optimizor
{
protected:
	Function* function;
	Constraints* constraints;
public:
	Optimizor() : function(nullptr), constraints(nullptr){}
	~Optimizor() {}

	virtual void Optimize(const char* results_path = nullptr) = 0;
};

#endif // !_Mathematical_Tools_Optimization_Optimizor_
