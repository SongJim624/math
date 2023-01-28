#include <vector>

class Distribution
{
public:
    virtual void density(const std::vector<size_t>& size, const float * variable, float * results) = 0;
    virtual void sampling(size_t size, float * results) = 0;
    virtual ~Distribution(){};
};

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
    virtual void density(const std::vector<size_t>& size, const float * variable, float * results);
    virtual void sampling(size_t size);
};