#include "../optimizer.h"
#include <functional>

#ifndef _SJML_Optimization_Gentic Algorithms_
#define _SJML_Optimization_Gentic_Algorithms_
class Genetic : public Optimizer
{
protected:
    class Population;

protected:
    virtual void Select(Population& population) = 0;
    virtual void Reproduce(Popoluation& population) = 0;

public:
    virtual std::vector<std::vector<float>> Optimize(Information *) = 0;
    virtual ~Genetic(){};
};

class Genetic::Population
{
public:
    class Individual;

protected:
    virtual void fitness(const float * decisions) = 0;

public:
    ~virtual Population(){};
};

class Genetic::Population::Individual
{
public:
    float * decisions, *objectives;
};
#endif // !_SJML_Optimization_Gentic Algorithms_