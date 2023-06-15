#include "unsga.hpp"

#ifndef _MATH_OPTIMIZATION_UNSGA_POINT_
#define _MATH_OPTIMIZATION_UNSGA_POINT_
template<typename T, class allocator>
UNSGA<T, allocator>::Population::Reference::Point::Point(const std::vector<T, allocator>& location) :
    location_(location) ,
    count(0),
    associated({})
{
}

template<typename T, class allocator>
T UNSGA<T,allocator>::Population::Reference::Point::distance(const std::vector<T, allocator>& point) const
{
    auto temporary = location_;
    temporary = (-dot(location_, point) / dot(location_, location_)) * location_ + temporary;
    return sqrt(dot(temporary, temporary));
}
#endif //!_MATH_OPTIMIZATION_UNSGA_POINT_