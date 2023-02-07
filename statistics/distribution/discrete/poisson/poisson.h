#include "../../../distribution/distribution.h"

#ifndef _SJML_STATISTICS_DISTRIBUTION_DISCRETE_POISSON_
#define _SJML_STATISTICS_DISTRIBUTION_DISCRETE_POISSON_

class Poisson : public Distribution
{
protected:
    float lambda_;

public:
    Poisson(float lambda);
    virtual ~Poisson(){};

public:
    virtual void density(size_t length, const float * locations, float * results);
    virtual void probability(size_t length, const float * locations, float * results);
    virtual void sampling(size_t length, float * results);
};

#endif //! _SJML_STATISTICS_DISTRIBUTION_DISCRETE_POISSON_