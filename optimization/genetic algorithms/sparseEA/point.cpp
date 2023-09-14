#include "unsga.h"

double dot(size_t length, const double* left, const double* right)
{
    auto temporary = std::unique_ptr<double[], decltype(&math::free)>{ math::allocate(length), math::free };
    math::mul(length, left, right, temporary.get());

    return std::accumulate(temporary.get(), temporary.get() + length, 0);
}


UNSGA::Population::Reference::Point::Point(size_t dimension, const double * location) :
    dimension_(dimension), count(0), location_(math::allocate(dimension_))
{
    math::copy(dimension_, location, 1, location_, 1);
}

double UNSGA::Population::Reference::Point::distance(const double * point) const
{
    auto temporary = std::unique_ptr<double[], decltype(&math::free)>{ math::allocate(dimension_), math::free };
    math::copy(dimension_, location_, 1, temporary.get(), 1);

    double fraction = -math::dot(dimension_, location_, 1, point, 1) / math::dot(dimension_, location_, 1, location_, 1);
    math::xpby(dimension_, point, 1, fraction, temporary.get(), 1);
    return std::sqrt(math::dot(dimension_, temporary.get(), 1, temporary.get(), 1));
}
