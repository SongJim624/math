#include "unsga.h"

UNSGA::Reference::Point::Point(const float * location)
    : location_(location), count_(0), associated_({})
{
}

UNSGA::Reference::Point::~Point()
{
    location_ = nullptr;
}

float UNSGA::Reference::Point:: distance(const float * point) const
{
    float * temporary = mkl_malloc(size * sizeof(float), 64);
    cblas_scopy(size, point, 1, temporary, 1);

    float coefficient = cblas_sdot(size, location_, 1, point, 1) / cblas_sdot(size, location_, 1, location_, 1);
    cblas_saxpy(size, -coefficent, location_, 1, temporary, 1);

    float result = sqrtf(cblas_sdot(size, temporary, 1, temporary, 1));
    mkl_free(temporary);
    temporary = nullptr;
    return result;
}