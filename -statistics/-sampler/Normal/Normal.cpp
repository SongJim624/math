#include "Normal.h"
#define Location SJML::Statistics::Sampler::Normal

Location::Normal(const float& mu, const float& sigma)
	: mu(mu), sigma(sigma), stream(nullptr)
{
    vslNewStream(&stream, VSL_BRNG_MCG31, (unsigned long)time(NULL));
}

Location::~Normal()
{
    vslDeleteStream(&stream);
    stream = nullptr;
}

void Location::sample(float * X, long size)
{
    vsRngGaussian(VSL_RNG_GAUSSIAN_METHOD_BOXMULLER,
        stream,  size, X, mu, sigma);
	return;
}