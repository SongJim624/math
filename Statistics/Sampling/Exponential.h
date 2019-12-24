#include "Sampling.h"

template<typename T>
class Exponential : public PDF<T>, public CDF<T>, public Sample<T>  
{
private:    
    T lamada;

private:
    std::vector<T> pdf(const std::vector<T>& X);
    std::vector<T> cdf(const std::vector<T>& X);
//    std::vector<T> sample(const std::vector<X>& Y)
public:
    Exponential<T> (const T& lamada);
};