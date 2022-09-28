#include <vector>
#include <mkl.h>

class Broyden
{
private:
    size_t scale_;
    //tolerance, two types
    float relative_, absolute_;
    //non-linear system to be solved
    std::function<void(float *, float *, size_t)> target_;

public:
    void Solve(const float * initial, float * result);

public:
    Broyden(std::function<void(float *, float *, size_t)>& target, size_t, float, float);
};