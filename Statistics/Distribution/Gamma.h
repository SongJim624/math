#include "Distribution.h"

class Gamma : public Distribution
{
private:    
    float alpha, beta;

private:
    std::vector<float> pdf(const std::vector<float>& X);
    std::vector<float> cdf(const std::vector<float>& X);
private:
    Gamma(const float& alpha, const float& beta)
    : alpha(alpha), beta(beta)
    {}

public:
    static Distribution* Gamma_Construct(const float& alpha, const float& beta)
    {
        return new Gamma(alpha, beta);
    }
};

_declspec (dllexport) Distribution* Gamma(const float& alpha, const float& beta)
{
    return Gamma::Gamma_Construct(alpha, beta);
}