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
    float Search(Function* function, float * X0, float * direction);

public:
    Powell(Function* function, const long& decisions, float * initial);
    ~Powell();

    virtual void Optimize(const char * result);

};
#endif //! _POWELL_