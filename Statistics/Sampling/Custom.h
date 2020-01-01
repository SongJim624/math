#include "Sampling.h"

template<typename T>
class Custom : public Distribution
{
private:
    std::vector<double> pdf(const std::vector<double>& X);
    std::vector<double> cdf(const std::vector<double>& X);


public:
    Custom(void* func);
};