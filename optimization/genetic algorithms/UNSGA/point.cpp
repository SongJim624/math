#include "unsga.h"

UNSGA::Population::Reference::Point::Point(size_t dimension, const double * location) :
    dimension_(dimension), count(0), location_(math::allocate(dimension_))
{
    math::copy(dimension_, location, 1, location_, 1);
}

double UNSGA::Population::Reference::Point::distance(const double * point) const
{
    auto temporary = std::unique_ptr<double[], decltype(&math::free)>{math::allocate()};

    auto temporary = location_;
    temporary = (-math::dot(location_, point) / math::dot(location_, location_)) * location_ + temporary;
    return std::sqrt(math::dot(temporary, temporary));
}
