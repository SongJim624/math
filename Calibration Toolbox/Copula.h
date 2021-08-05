#include <unordered_map>
#include <string>
#include "Distribution.h"
#include "Empirical.h"
#include "KDE.h"

class Copula : public Distribution
{
private:
    std::unordered_map<std::string, Distribution*> marginals;

private:
//or other copulas
    void Gaussian(float * X, float * Y, long N);

public:
    Copula(std::vector<std::string>& names, std::vector<float>& data, const std::vector<bool>& integer);
    
public:
    void density(float * X, float * Y, long N);
    void sampling(float * X, long N);
};