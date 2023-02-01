#include "unsga.h"

UNSGA::Reference::Point::Point(const float * location, std::shared_ptr<Configuration> configuration)
    : location_(location), configuration_(configuration), count(0), associated({})
{
}

UNSGA::Reference::Point::~Point()
{
    location_ = nullptr;
}

float UNSGA::Reference::Point:: distance(const float * point) const
{
    size_t dimension = configuration_->dimension;

    float * temporary = (float *) mkl_malloc(dimension * sizeof(float), 64);
    cblas_scopy(dimension, point, 1, temporary, 1);

    float coefficient = cblas_sdot(dimension, location_, 1, point, 1) / cblas_sdot(dimension, location_, 1, location_, 1);
    cblas_saxpy(dimension, -coefficient, location_, 1, temporary, 1);

    float result = sqrtf(cblas_sdot(dimension, temporary, 1, temporary, 1));
    mkl_free(temporary);
    temporary = nullptr;
    return result;
}