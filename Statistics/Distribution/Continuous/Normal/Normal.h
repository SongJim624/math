#include "../Continuous.h"

class Normal : public Distribution
{
protected:
    constexpr float pi = 3.1415926f;
    float mu, sigma;

public:
    Normal(float mu, float sigma);
    virtual ~Normal();

public:
    virtual float density(float);
    virtual float distribution(float);
};
