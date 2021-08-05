#include <math.h>
#include <mkl.h>

#ifndef _SJML_STATISTICS_DISTRIBUTION_CONTINUOUS_
#define _SJML_STATISTICS_DISTRIBUTION_CONTINUOUS_
namespace SJML
{
    namespace Statistics
    {
        namespace Distribution
        {
            namespace Continuous
            {
                class Continuous
                {
                public:
                    virtual ~Continuous(){};

                public:
                    virtual void density(float * X, float * Y, long size) = 0;
                    virtual void distribution(float * X, float * Y, long size) = 0;
                };
            };
        };
    };
};
#endif //! _SJML_STATISTICS_DISTRIBUTION_CONTINUOUS_