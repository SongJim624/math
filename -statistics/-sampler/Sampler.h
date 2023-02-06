#include <time.h>
#include "../Distribution/Distribution.h"

#ifndef _SJML_STATISTICS_SAMPLER_
#define _SJML_STATISTICS_SAMPLER_
namespace SJML
{
    namespace Statistics
    {
        namespace Sampler
        {
            class Sampler
            {
            public:
                virtual void Sample(float * samples long size) = 0;
            };
        }
    }
}
#endif //! _SJML_STATISTICS_SAMPLER_