//Box - Muller Method
//Single number
#include "Distribution.h"

template<typename T>
class Normal : public dist<T>
{
private:
    T mu, sigma;

private:
    std::vector<T> pdf(const std::vector<T>& X);
    std::vector<T> cdf(const std::vector<T>& X);

public:
    Normal<T>(const T& mu, const T& sigma)
    : mu(mu), sigma(sigma)
    {};    
};