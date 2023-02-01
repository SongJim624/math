#include "../optimizer.h"
#include <functional>

#ifndef _SJML_Optimization_Gentic Algorithms_
#define _SJML_Optimization_Gentic_Algorithms_
class Genetic : public Optimizer
{
protected:
    class Individual;

public:
    virtual std::shared_ptr<Result> Optimize(Objective * objective) = 0;
    virtual ~Genetic(){};
};

class Genetic::Individual
{
public:
    float * decisions, *objectives;

public:
//the reason of using the int type but not bool
//when applying the non dorminant sort, if bool return only, the operator > needed be called again.
    virtual int operator < (const Individual& individual) const = 0;

public:
    Individual() : decisions(nullptr), objectives(nullptr) {}
    ~Individual() {}
};
#endif // !_SJML_Optimization_Gentic Algorithms_