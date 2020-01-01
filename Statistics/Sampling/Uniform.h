#include "Distribution.h"

template<typename T>
class Uniform : public dist<T> 
{
private:
    T a, b;

private:
    std::vector<T> pdf(const std::vector<T> &X);
    std::vector<T> cdf(const std::vector<T> &X);

public:
    Uniform<T>(const T& a, const T& b)
    :a(a), b(b)
    {}; 
};
