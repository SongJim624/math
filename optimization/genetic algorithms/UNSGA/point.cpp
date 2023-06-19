#include "unsga.h"

UNSGA::Population::Reference::Point::Point(const Matrix& location) :
    location_(location) ,
    count(0),
    associated({})
{
}

double UNSGA::Population::Reference::Point::distance(const Matrix<double>& point) const
{
    auto temporary = location_;
    temporary = (-dot(location_, point) / dot(location_, location_)) * location_ + temporary;
    return sqrt(dot(temporary, temporary));
}