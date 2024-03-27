#include <time.h>
#include "../Distribution/Distribution.h"

#ifndef _SJML_STATISTICS_SAMPLER_
#define _SJML_STATISTICS_SAMPLER_
class Sampler
{
public:
    virtual double * sample(size_t length, double * samples) = 0;
};



#endif //! _SJML_STATISTICS_SAMPLER_