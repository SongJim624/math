#include "configuration.hpp"
#include "individual.hpp"

#ifndef _MATH_OPTIMIZATION_UNSGA_POINT_
#define _MATH_OPTIMIZATION_UNSGA_POINT_
template<typename T>
class Point
{
private:
    Vector<T> location_;

public:
	size_t count;
	Series<T> associated;

public:
	//perpendicular distance for a point to the reference line
	T distance(const Vector<T>& point) const;

    Point(const Vector<T>& location);
};

template<typename T>
Point<T>::Point(const Vector<T>& location) : location_(location) , count(0)
{
}

template<typename T>
T Point<T>::distance(const Vector<T>& point) const {
    auto temporary = location_;
    temporary = (-dot(location_, point) / dot(location_, location_)) * location_ + temporary;
    return sqrt(dot(temporary, temporary));
}
#endif //!_MATH_OPTIMIZATION_UNSGA_POINT_