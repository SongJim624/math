#include "unsga.h"

Point::Point(const std::vector<double>& location)
    : location_(location) , count(0), associated({})
{
}

double Point::distance(const std::vector<double>& point) const
{
//    auto temporary = location_;
//    std::inner_product(location_.begin(), location_.end(), point.begin(), 0)
//    temporary = (-dot(location_, point) / dot(location_, location_)) * location_ + temporary;
//    return sqrt(dot(temporary, temporary));
    return 0;
}