#ifndef _DISTRIBUTION_
#define _DISTRIBUTION_
class Distribution
{
public:
    class Sampler;

public:
    virtual void density(size_t length, const float * locations, float * values) = 0;
    virtual void probability(size_t length, const float * locations, float * values) = 0;
    virtual void sampling(size_t length, float values) = 0;

public:
    virtual ~Distribution(){};
};

class Distribution::Sampler
{
public:
    virtual void sampling(size_t lenght, float * samples) = 0;

public:
    virtual ~Sampler(){};
};

//Distribution * factory();
#endif //!_DISTRIBUTION_