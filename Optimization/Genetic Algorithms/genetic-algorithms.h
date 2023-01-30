#include "../optimizer.h"
#include <functional>

#ifndef _SJML_Optimization_Gentic Algorithms_
#define _SJML_Optimization_Gentic_Algorithms_
class Genetic : public Optimizer
{
public:
    class Individual;
    class Reproductor;

public:
    virtual std::vector<std::vector<float>> Optimize(Information *) = 0;
    virtual ~Genetic(){};
};

class Genetic::Individual
{
protected:
    static Information * information_;
    float* decisions_, * objectives_;

public:
    Individual() : decisions_(nullptr), objectives_(nullptr){};
    ~Individual(){};

public:
    static void Initialize(Information * information) { information_  = information;};
    static void Finalize() {information_ = nullptr; };
};

class Genetic::Reproductor
{
protected:
    static Information * information_;

public:
    Reproductor(){};
    virtual ~Reproductor() {}

public:
    virtual void Reproduce(std::list<Individual*>& solution, std::list<Individual*>& population) = 0;

public:
    static void Initialize(Information * information) { information_  = information;};
    static void Finalize() { information_  = nullptr;};
};
#endif // !_SJML_Optimization_Gentic Algorithms_