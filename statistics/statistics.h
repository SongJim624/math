#ifndef _DISTRIBUTION_
#define _DISTRIBUTION_
namespace math
{
    template<typename T>
    class Distribution
    {
    public:
        virtual void density(size_t, const T*, T*) = 0;
        virtual void probability(size_t, const T*, T*) = 0;
    };

    template<class T>
    class Sampler
    {
    public:
        virtual void sample(size_t, T*) = 0;
    };
}
#endif //!_DISTRIBUTION_