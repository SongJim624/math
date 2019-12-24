#include "PDF.h"

void PDF::Normal(const float & mu, const float & sigma, const float &x, float &pdf)
{
    pdf = exp(-0.5 * (x - mu) * (x - mu) / (sigma * sigma)) / (sqrtf(2 * pi) * sigma);
}