#include "Distribution.h"

template<typename T>
class Exponential : public dist<T>  
{
private:    
    T lamada;

private:
    std::vector<T> pdf(const std::vector<T>& X);
    std::vector<T> cdf(const std::vector<T>& X);
public:
    Exponential<T> (const T& lamada);
};