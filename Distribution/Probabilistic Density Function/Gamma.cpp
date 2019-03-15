#include "PDF.h"

void Gamma(const float& alpha, const float &beta, const float &x, float &pdf)
{
    pdf = powf(beta, alpha) * powf(x, alpha - 1) * expf(-beta * x) / tgammaf(alpha);
}