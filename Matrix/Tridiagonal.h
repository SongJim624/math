#include "Matrix.h"

class Tridiagonal : public Matrix
{
private:
    float* A, * B, * C;

public:
    Tridiagonal(const std::vector<float>&);
    ~Tridiagonal();

public:
    virtual float& operator()(long r, long c);
    float* Diagonal(long n);
};