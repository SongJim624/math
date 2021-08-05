#include "../Sampler.h"
#include <time.h>


#ifndef _SJML_STATISTICS_SAMPLER_
#define _SJML_STATISTICS_SAMPLER_
namespace SJML
{
    namespace Statistics
    {
        namespace Sampler
        {
            class Normal : public Sampler
            {
            private:
                const float mu, sigma;
                VSLStreamStatePtr stream;

            public:
                virtual void Sample(float * samples long size) = 0;
            };
        }
    }
}
#endif //! _SJML_STATISTICS_SAMPLER_