#include "Sensitivity Analysis.h"

class Sobol : public SA
{
private:
    size_t N;
    Sampling* sampling;

private:
    virtual std::vector<float> analysis(Function func, std::vector<Distribution*>& parameter);

private:
    Sobol ()
    {

    }

public:
    static SA* Construct()
    {

    }    
};