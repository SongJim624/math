//This for the continuous distribution
#include "Sampling.h"

template<typename T>
class Uniform : public PDF<T>, public CDF<T>, public Sample<T> 
{
private:
    T a, b;

private:
    std::vector<T> pdf(const std::vector<T> &X);
    std::vector<T> cdf(const std::vector<T> &X);

public:
    Uniform<T>(const T& a, const T& b);    
};
