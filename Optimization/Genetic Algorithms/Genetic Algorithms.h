
#ifndef _Mathematical_Tools_Optimization_Gentic Algorithms_
#define _Mathematical_Tools_Optimization_Gentic Algorithms_

#include "../Optimizor.h"
#include <stdlib.h>

class Individual
{
public:
    static long decision_size, objective_size;
    static float* upper, * lower;
    static bool* integer;

    float voilate;
    float* decisions, * objectives;

public:
    Individual();
    ~Individual();
};

class Genetic
{
public:
    ~Genetic() {}

    virtual void Cross(Individual& father, Individual& mother, Individual& son, Individual& daughter) = 0;
    virtual void Mutation(Individual& individual) = 0;
};

class Genetic_Algorithm : public Optimizor
{
protected:
    Genetic* genetic;

public:
    Genetic_Algorithm() : genetic(nullptr) {}
    ~Genetic_Algorithm() {}

    virtual void Optimize(const char* results_path = nullptr) = 0;
};
#endif // !_Mathematical_Tools_Optimization_Gentic Algorithms_