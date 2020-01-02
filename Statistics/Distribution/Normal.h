//Box - Muller Method
//Single number
#include "Distribution.h"

class Normal : public Distribution
{
private:
    float mu, sigma;

private:
    std::vector<float> pdf(const std::vector<float>& X);
    std::vector<float> cdf(const std::vector<float>& X);

private:
	Normal(const float& mu, const float& sigma)
		: mu(mu), sigma(sigma)
	{};
public:
	static Distribution* Normal_Construct(const float& mu, const float& sigma)
	{
		return new Normal(mu, sigma);
	};
};

_declspec (dllexport) Distribution* _stdcall Normal(const float& mu, const float& sigma)
{
	return Normal::Normal_Construct(mu, sigma);
}