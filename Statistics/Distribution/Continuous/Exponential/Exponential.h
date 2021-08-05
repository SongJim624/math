#include "../Continuous.h"

#ifndef _SJML_STATISTICS_DISTRIBUTION_CONTINUOUS_EXPONENTIAL_
#define _SJML_STATISTICS_DISTRIBUTION_CONTINUOUS_EXPONENTIAL_
namespace SJML
{
    namespace Statistics
    {
        namespace Distribution
        {
            namespace Continuous
            {
                class Exponential : public Continuous
                {
                protected:
                    const float lambda;

                public:
					Exponential(const float& lambda);
                    virtual ~Exponential();

                public:
                    virtual void density(float * X, float * Y, long size);
                    virtual void distribution(float * X, float * Y, long size);
                };
            }
        }
    }
}
#endif //! _SJML_STATISTICS_DISTRIBUTION_CONTINUOUS_EXPONENTIAL_