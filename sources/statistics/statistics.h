#ifndef _DISTRIBUTION_
#define _DISTRIBUTION_
namespace math
{
    class distribution
    {
    public:
        virtual void density    (size_t size, const double*, double*) = 0;
        virtual void probability(size_t size, const double*, double*) = 0;
    };

    class sampler
    {
    public:
        virtual void sample(size_t, double*) = 0;
    };
}
#endif //!_DISTRIBUTION_