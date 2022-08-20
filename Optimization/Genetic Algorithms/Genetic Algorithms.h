#include "../Optimizor.h"
#include <functional>

#ifndef _SJML_Optimization_Gentic Algorithms_
#define _SJML_Optimization_Gentic_Algorithms_

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
    virtual ~Genetic() {}

    virtual void Cross(Individual& father, Individual& mother, Individual& son, Individual& daughter) = 0;
    virtual void Mutation(Individual& individual) = 0;
};

class Genetic_Algorithm : public Optimizor
{
protected:
    Genetic* genetic;

public:
    Genetic_Algorithm() : genetic(nullptr) {}
    virtual ~Genetic_Algorithm() {}

    virtual void Optimize(const char* results_path = nullptr) = 0;
};

class UNSGA
{
private:
    size_t decisions_, objectives_;
    float * upper_, * lower_;
    bool * integer_;

private:
    struct Individual;

private:

private:


};


#endif // !_SJML_Optimization_Gentic Algorithms_