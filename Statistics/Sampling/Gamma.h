#include "Distribution.h"

template<typename T>
class Gamma : public dist<T>  
{
private:    
    T alpha, beta;

private:
    std::vector<T> pdf(const std::vector<T>& X);
    std::vector<T> cdf(const std::vector<T>& X);
public:
    Gamma<T> (const T& alpha, const T& beta);
};