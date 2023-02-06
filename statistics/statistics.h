class Distribution
{
public:
    virtual void density(size_t length, const float * locations, float * values) = 0;
    virtual float probability(size_t length, const float * locations, float * values) = 0;

public:
    virtual ~Distribution(){};
};

class Sampler
{
protected:
    Distribution * distribution_;

public:
    virtual void Sample(size_t lenght, float * samples) = 0;

public:
    Sampler(Distribution * distribution) : distribution_(distribution){};
    virtual ~Sampler() { distribution_ = nullptr; };
};