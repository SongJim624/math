#include <math.h>
#include <mkl.h>

#ifndef _SJML_STATISTICS_DISTRIBUTION_DISCRETE_
#define _SJML_STATISTICS_DISTRIBUTION_DISCRETE_
namespace SJML
{
    namespace Statistics
    {
        namespace Distribution
        {
            namespace Discrete
            {
                class Discrete
                {
                public:
                    virtual ~Discrete(){};

                public:
                    virtual void function(long * X, float * Y, long size) = 0;
                    virtual void distribution(long * X, float * Y, long size) = 0;
                };
            };
        };
    };
};
#endif //! _SJML_STATISTICS_DISTRIBUTION_DISCRETE_