#include "Sampling.h"

template<typename T>
class Gamma : public PDF<T>, public CDF<T>, public Sample<T>  
{
private:    
    T alpha, beta;

private:
    std::vector<T> pdf(const std::vector<T>& X);
    std::vector<T> cdf(const std::vector<T>& X);
//    std::vector<T> sample(const std::vector<X>& Y)
public:
    Gamma<T> (const T& alpha, const T& beta);
};