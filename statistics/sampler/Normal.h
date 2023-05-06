#include "Sampling.h"

class Box_Muller : public Sampling
{
private:
    float mu, sigma;
    int type;

private:
    virtual std::vector<float> sampling(const size_t& N, Distribution* dist = nullptr);
    virtual void ChangeParameter(const float& a, const float& b, const float& c, const float& d);
private:
    Box_Muller(const float&mu, const float& sigma, const size_t& type)
    : mu(mu), sigma(sigma), type(type)
    {}

public:
    static Sampling* Construct(const float& mu, const float&sigma, const size_t &type)
    {
        return new Box_Muller(mu, sigma, type);
    }
};

std::vector<float> sampling1(const size_t& N, const float& mu, const float& sigma);
std::vector<float> sampling2(const size_t& N, const float& mu, const float& sigma);
std::vector<float> sampling3(const size_t& N, const float& mu, const float& sigma);