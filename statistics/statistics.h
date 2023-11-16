#ifndef _DISTRIBUTION_
#define _DISTRIBUTION_
namespace math
{
    template<typename T>
    class distribution
    {
    public:
        virtual void density(size_t size, const T* samples, size_t sinc, T* densities, size_t dinc) const = 0;
        virtual void probability(size_t size, const T* sample, )
    };

    class distribution
    {
    public:
        virtual void density(size_t size, const double*)

    };


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