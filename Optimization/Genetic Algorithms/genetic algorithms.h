#include "../optimizer.h"
#include <functional>

#ifndef _SJML_Optimization_Gentic Algorithms_
#define _SJML_Optimization_Gentic_Algorithms_
class Genetic : public Optimizer
{
protected:
    class Individual;
    class Population;

protected:
    virtual void Select(Population* population) = 0;
    virtual void Reproduce(Population* population) = 0;

public:
    virtual std::vector<std::vector<float>> Optimize(Information *) = 0;
    virtual ~Genetic(){};
};

class Genetic::Population
{
protected:
    virtual void fitness(Individual*) = 0;
    virtual void Sort() = 0;

public:
    virtual ~Population(){};
};

class Genetic::Individual
{
public:
    float * decisions, *objectives;

public:
//the reason of using the int type but not bool
//when applying the non dorminant sort, if bool return only, the operator > needed be called again.
    virtual int operator < (const Individual& individual) const = 0;
};
#endif // !_SJML_Optimization_Gentic Algorithms_