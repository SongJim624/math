#include "Distribution.h"

class Exponential : public Distribution  
{
private:    
    float lamada;

private:
    std::vector<float> pdf(const std::vector<float>& X);
    std::vector<float> cdf(const std::vector<float>& X);
private:
    Exponential (const float& lamada)
    : lamada(lamada)
    {};

public:
    static Distribution* Exponential_Construct(const float& lamada)
    {
        return new Exponential(lamada);
    }    
};

_declspec (dllexport) Distribution* _stdcall Exponential(const float& lamada)
{
    return Exponential::Exponential_Construct(lamada);
}