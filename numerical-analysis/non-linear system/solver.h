#include <mkl.h>
#include <functional>

class Solver
{
public:
    typedef std::function<void(const float *, size_t, float *, size_t)> Target;

    struct Defination
    {
        std::function<void(const float *, size_t, float *, size_t)> objective;
        std::function<void(std::function<void(const float *, size_t, float *, size_t)>&, int, int, const float *, float *)> jacobi_;
        size_t row, column;
        size_t mode;
        size_t maximum;
        float criteria, tolerance;
    };

private:
    int row_, column_, maximum_;
    float criteria_, tolerance_;

private:
    Target objective_;
    std::function<void(Target&, const float *, float *)> jacobi_;
    std::function<void(const float *, float *)> solver_;

public:
    Solver(Defination& defination);

public:
    void Solve(const float *, float *);

//control parameters
public:
    int& maximum();
    float& criteria();
    float& tolerance();
};