#include "../Discrete.h"

#ifndef _SJML_STATISTICS_DISTRIBUTION_DISCRETE_POISSON_
#define _SJML_STATISTICS_DISTRIBUTION_DISCRETE_POISSON_
namespace SJML
{
    namespace Statistics
    {
        namespace Distribution
        {
            namespace Discrete
            {
                class Poisson : public Discrete
                {
                protected:
                    float lambda;

                public:
					Poisson(const float& lambda);
                    virtual ~Poisson(){};

                public:
                    virtual void density(long * X, float * Y, long size);
                    virtual void probability(long * X, float * Y, long size);
                };
            };
        };
    };
};
#endif //! _SJML_STATISTICS_DISTRIBUTION_DISCRETE_POISSON_