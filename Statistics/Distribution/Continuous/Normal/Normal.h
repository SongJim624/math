#include "../Continuous.h"

#ifndef _SJML_STATISTICS_DISTRIBUTION_CONTINUOUS_NORMAL_
#define _SJML_STATISTICS_DISTRIBUTION_CONTINUOUS_NORMAL_
namespace SJML
{
    namespace Statistics
    {
        namespace Distribution
        {
            namespace Continuous
            {
                class Normal : public Continuous
                {
                protected:
                    const float pi, mu, sigma;

                public:
					Normal(const float& mu, const float& sigma);
                    virtual ~Normal();

                public:
                    virtual void density(float * X, float * Y, long size);
                    virtual void distribution(float * X, float * Y, long size);
                };
            }
        }
    }
}
#endif //! _SJML_STATISTICS_DISTRIBUTION_CONTINUOUS_NORMAL_