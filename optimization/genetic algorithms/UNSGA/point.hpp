#include "configuration.hpp"

#ifndef _MATH_OPTIMIZATION_UNSGA_POINT_
#define _MATH_OPTIMIZATION_UNSGA_POINT_
template<typename T>
class Point
{
private:
    std::vector<T, Allocator<T>> location_;

public:
	size_t count;
	Series<T> associated;

private:
    T dot(const std::vector<T, Allocator<T>>& lhs, const T* rhs) {
        if(std::is_same<T, float>) {
            return cblas_sdot(lhs.size(), &lhs[0], 1, rhs, 1);
        }

        if(std::is_same<T, double>) {
            return cblas_ddot(lhs.size(), &lhs[0], 1, rhs, 1);
        }
    }

    void axpy(T a, const std::vector<T, Allocator<T>>& x, std::vector<T, Allocator<T>>& y) {
        if (std::is_same<T, float>){
            cblas_saxpy(x.size(), a, &x[0], 1, &y[0], 1);
        }

        if (std::is_same<T, double>){
            cblas_daxpy(x.size(), a, &x[0], 1, &y[0], 1);
        }
    }

public:
	//perpendicular distance for a point to the reference line
	T distance(const T* point) const {
        auto temporary = location_;
        axpy(-dot(location_, point) / dot(location_, &location_[0]), location_, temporary);
        return sqrt(dot(temporary, &temporary[0]));
    }

    Point(const T* address, size_t length) : location_(std::vector<T, Allocator<T>>(address, address + length)) {
    }
};
#endif //!_MATH_OPTIMIZATION_UNSGA_POINT_