#include "unsga.h"

float dot(const UNSGA::vector& lhs, const UNSGA::vector& rhs)
{
    assert(lhs.size() == rhs.size(), "dot function for vector: size not equal");
    return cblas_sdot(lhs.size(), &lhs[0], 1, &rhs[0], 1);
}

UNSGA::Reference::Point::Point(const float * location, std::shared_ptr<Configuration> configuration)
    : configuration_(configuration), count(0), associated({})
{
    location_ = std::vector<float, UNSGA::Allocator<float>>(configuration->dimension);
    cblas_scopy(configuration->dimension, location, 1, &location_[0], 1);
}

float UNSGA::Reference::Point:: distance(const UNSGA::vector& point) const
{
    size_t dimension = configuration_->dimension;
    UNSGA::vector temporary(dimension);

    cblas_scopy(dimension, &point[0], 1, &temporary[0], 1);
    cblas_saxpy(dimension, -dot(location_, point) / dot(location_, location_), &location_[0], 1, &temporary[0], 1);

    return sqrtf(dot(temporary, temporary));
}