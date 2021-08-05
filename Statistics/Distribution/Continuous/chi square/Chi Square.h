#include "../Continuous.h"

#ifndef _SJML_STATISTICS_DISTRIBUTION_CONTINUOUS_CHISQUARE_
#define _SJML_STATISTICS_DISTRIBUTION_CONTINUOUS_CHISQUARE_
namespace SJML
{
    namespace Statistics
    {
        namespace Distribution
        {
            namespace Continuous
            {
                class ChiSquare : public Continuous
                {
                protected:
                    const long k;

                public:
					ChiSquare(const long& dF);
                    virtual ~ChiSquare();

                public:
                    virtual void density(float * X, float * Y, long size);
                    virtual void distribution(float * X, float * Y, long size);
                };
            }
        }
    }
}
#endif //! _SJML_STATISTICS_DISTRIBUTION_CONTINUOUS_CHISQUARE_