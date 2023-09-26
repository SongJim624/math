#include <mkl.h>
#include <functional>

class LevenbergMarquardt
{
typedef std::function<void(const float *, size_t, float *, size_t)> Target;
private:
    Jacobian(Target&, size_t, size_t, double*, double*);

private:
    size_t row_, column_;
    Target objective_;
    //if the Jacobian calculator is not defined, use the numerical one
    std::function<void(Target&, size_t, size_t, double*, double*)> jacobi_;


public:
    LevenbergMarquardt();

public:
    void solve(const float * initial, float * solution, size_t size);
};