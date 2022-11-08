#include <vector>
#include <mkl.h>
#include <functional>
#include <iostream>

class Broyden
{
private:
    int scale_, maximum_;
    float secant_, value_;
    std::function<void(const float *, size_t, float *, size_t)> target_;
    std::function<void(size_t, float*, const float*, float*, const float*)> updator_;

private:
    void Updator(size_t);

public:
    void Solve(const float * initial, float * result);

public:
    Broyden(std::function<void(const float *, size_t, float *, size_t)>, size_t, float, float, size_t, size_t);
};