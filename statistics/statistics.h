#ifndef _DISTRIBUTION_
#define _DISTRIBUTION_
namespace math
{
    class Distribution
    {
    public:
        virtual void density(size_t, const double*, double*) = 0;
        virtual void probability(size_t, const double*, double*) = 0;
    };

    template<class T>
    class Sampler
    {
    public:
        virtual void sample(size_t, T*) = 0;
    };
}
#endif //!_DISTRIBUTION_