#include "../Continuous.h"

#ifndef _SJML_STATISTICS_DISTRIBUTION_CONTINUOUS_GAMMA_
#define _SJML_STATISTICS_DISTRIBUTION_CONTINUOUS_GAMMA_
namespace SJML
{
    namespace Statistics
    {
        namespace Distribution
        {
            namespace Continuous
            {
                class Gamma : public Continuous
                {
                protected:
                    const float alpha, beta;

                public:
					Gamma(const float& alpha, const float& beta);
                    virtual ~Gamma();

                public:
                    virtual void density(float * X, float * Y, long size);
                    virtual void distribution(float * X, float * Y, long size);
                };
            }
        }
    }
}
#endif //! _SJML_STATISTICS_DISTRIBUTION_CONTINUOUS_GAMMA_