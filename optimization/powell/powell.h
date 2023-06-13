#include "../Optimizor.h"
#include <mkl.h>

#ifndef _POWELL_
#define _POWELL_
class Powell : public Optimizor
{
private:
//this is used for the traditional Powell
    double * decision_, * initial_;
    std::list<double *> directions;

    float * values;

private:
    double Search(Function* function, double * initial, double * direction);

public:
    Powell(Function* function, size_t decisions, double * initial);
    ~Powell();

public:
    virtual std::shared_ptr<Result> Optimize(Objective * objective);
};
#endif //! _POWELL_