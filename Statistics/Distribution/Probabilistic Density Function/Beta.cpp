#include "PDF.h"

void Beta(const float &alpha, const float &beta, const float &x, float &pdf)
{
    pdf = tgammaf(alpha + beta) * powf(x, alpha - 1) * powf(1 - x, beta - 1) / (tgammaf(alpha) * tgammaf(beta));
}
