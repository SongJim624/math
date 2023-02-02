#include "unsga.h"

float dot(const std::vector<float>& lhs, const std::vector<float>& rhs)
{
    assert(lhs.size() == rhs.size(), "dot function for vector: size not equal");
    return cblas_sdot(lhs.size(), &lhs[0], 1, &rhs[0], 1);
}

UNSGA::Reference::Point::Point(const float * location, std::shared_ptr<Configuration> configuration)
    : configuration_(configuration), count(0), associated({})
{
    location_ = std::vector<float, UNSGA::Allocator<float>>(configuration->dimension);
    cblas_scopy(location.size(), location, 1, &location_[0], 1);
}

UNSGA::Reference::Point::~Point()
{
}

float UNSGA::Reference::Point:: distance(const std::vector<float>& point) const
{
    size_t dimension = configuration_->dimension;
    std::vector<float, UNSGA::Allocator<float>> temporary(dimension);

    cblas_scopy(dimension, &point[0], 1, &temporary[0], 1);
    cblas_saxpy(dimension, -dot(location_, point) / cblas_sdot(location_, location_), location_, 1, temporary, 1);

    return sqrtf(dot(temporary, temporary));
}