#include "Sampling.h"
//============================================================================================
class Metropolis_Hasting : public Sampling
{
private:
    Sampling* proposal_sampling;
    Distribution* proposal_dist;

    float X0;

private:
    virtual std::vector<float> sampling(const size_t& N, Distribution* dist);

    virtual void ChangeParameter(const float& a, const float& b, const float& c, const float& d)
        {
        printf("warning! No parameters could be changed! /n");
        return;
    }

private:
//The original metropolis hasting algorithm
    Metropolis_Hasting(const float& X0)
    :X0(X0)
    {
        proposal_sampling = Box_Muller(X0);
    }

//the independent metropolis hasting algorithm
    Metropolis_Hasting(const float X0, Distribution* dist)
    :
    {}

public:
    static Sampling* Construct(const float& X0)
    {
        return new Metropolis_Hasting(X0);
    }

    ~Metropolis_Hasting()
    {
        delete proposal;
    }
};