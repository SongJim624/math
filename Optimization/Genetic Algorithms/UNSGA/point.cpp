#include "unsga.h"

UNSGA::Reference::Point::Point(size_t division, const std::list<size_t>& location)
    : location_(nullptr), count_(0), associated_({})
{
    location_ = mkl_malloc(size * sizeof(float), 64);

    for(const auto& value : location)
    {
        location_++ = float(value);
    }

    cblas_sscal(size, 1.0 / division, location_, 1);
}

UNSGA::Reference::Point::~Point()
{
    mkl_free(location_);
    location_ = nullptr;
    associated_.clear();
}

size_t UNSGA::Reference::Point::count() const
{
    return count_;
}

void UNSGA::Reference::Point::Attach()
{
    count_++;
}

void UNSGA::Reference::Point::Attach(Individual * individual)
{
    associated_.push_back(individual);
}

Individual* UNSGA::Reference::Point::Detach()
{
    Individual* result = *associated_.begin();
    associated_.pop_front();
    count_++;

    return result;
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