#include <map>
#include <string>
#include "../Sampler.h"
#include "../../Distribution/Distribution.h"


#ifndef _METROPOLISHASTING_
#define _METROPOLISHASTING_
class MetropolisHasting
{
private:
    std::function<double * (size_t, double *)> sampler_;

public:
    class Option;

private:
    double sample(size_t length, double * samples);
    void Strategies(const std::string& name, const Option& options);

public:
    MetropolisHasting(const std::string& name, const Option& options);
};

class MetropolisHasting::Option
{
private:
    std::map<std::string, std::variant<double, std::vector<double>, std::string, bool>> options_;

public:
    auto operator [] (const std::string& name);
    auto operator [] (const std::string& name) const;
}
#endif _METROPOLISHASTING_

class Metropolis_Hasting : public Sampler
{
protected:
/*
* Independent Metropolis Hastings
*   Fixed proposal
*   Metropolis Hsating version of Accept and Reject Sampling
* Random Walk
* Langevin Algorithm
* DRAM
*/
    std::string type;
    std::map<std::string, > strategies;

protected:
    Sampler*  proposal;
    Distribution* p,  d;
    float X0;

private:
    void Sampling_Metropolis_Hasting(float * Y, long size);
    void Independent_Metropolis_Hasting(float * Y, long size);

};