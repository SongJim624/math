#include "Distribution.h"

class Exponential : public Distribution  
{
private:    
    float lamada;

private:
    std::vector<float> pdf(const std::vector<float>& X);
    std::vector<float> cdf(const std::vector<float>& X);
public:
    Exponential (const float& lamada)
    : lamada(lamada)
    {};
};