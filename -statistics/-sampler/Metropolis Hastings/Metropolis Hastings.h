#include <map>
#include <string>
#include "../Sampler.h"
#include "../../Distribution/Distribution.h"

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