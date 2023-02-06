#include "../Continuous.h"

#ifndef _SJML_STATISTICS_DISTRIBUTION_CONTINUOUS_UNIFORM_
#define _SJML_STATISTICS_DISTRIBUTION_CONTINUOUS_UNIFORM_
namespace SJML
{
    namespace Statistics
    {
        namespace Distribution
        {
            namespace Continuous
            {
                class Uniform : public Continuous
                {
                protected:
                    const float left, right;

                public:
					Uniform(const float& a, const float& b);
                    virtual ~Uniform();

                public:
                    virtual void density(float * X, float * Y, long size);
                    virtual void distribution(float * X, float * Y, long size);
                };
            }
        }
    }
}
#endif //! _SJML_STATISTICS_DISTRIBUTION_CONTINUOUS_UNIFORM_