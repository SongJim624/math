#include "../optimizor.h"
#include <mkl.h>

#ifndef _POWELL_
#define _POWELL_
class Powell : public math::Optimizor
{
private:
//this is used for the traditional Powell
    double * decision_, * initial_;
    std::list<double *> directions;

    double * values;

private:
    double Search(Function* function, double * initial, double * direction);

public:
    Powell();
    ~Powell();

public:
//    Powell(Function* function, size_t decisions, double * initial);
    virtual const Result& Optimize(Configuration& configuration);
};
#endif //! _POWELL_