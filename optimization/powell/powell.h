#include "../Optimizor.h"
#include <mkl.h>

#ifndef _POWELL_
#define _POWELL_
class Powell : public Optimizor
{
private:
//this is used for the traditional Powell
    float * decision, * initial;
    std::list<float *> directions;

    float * values;

private:
    float Search(Function* function, float * initial, float * direction);

public:
    Powell(Function* function, size_t decisions, float * initial);
    ~Powell();

public:
    virtual std::shared_ptr<Result> Optimize(Objective * objective);
};
#endif //! _POWELL_