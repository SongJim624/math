#include "Distribution.h"

class Uniform : public Distribution
{
private:
    float a, b;

private:
    std::vector<float> pdf(const std::vector<float> &X);
    std::vector<float> cdf(const std::vector<float> &X);

private:
    Uniform(const float& a, const float& b)
    :a(a), b(b)
    {}

public:
    static Distribution* Uniform_Construct(const float& a, const float& b)
    {
        return new Uniform(a, b);
    }
};

_declspec (dllexport) Distribution* _stdcall Uniform(const float& a, const float& b)
{
    return Uniform::Uniform_Construct(a, b);
}
