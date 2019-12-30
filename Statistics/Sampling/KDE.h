#include "Distribution.h"

template<typename T>
class KDE : public dist<T>  
{
private:    
    T alpha, beta;

private:
    std::vector<T> pdf(const std::vector<T>& X);
    std::vector<T> cdf(const std::vector<T>& X);
public:
    KDE<T> (const T& alpha, const T& beta);
};