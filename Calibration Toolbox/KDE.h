#include <limits>
#include <vector>
#include <mkl.h>
#include <time.h>
#include <algorithm>
#include <math.h>
#include <map>
#include <list>
#include <utility>
#include "Distribution.h"

#ifndef _KDE_
#define _KDE_
//This is only for one dimension
struct Proposal
{
    float sample, proposal, target;
    long location;

    Proposal(float& sample, float& proposal, float& target, long& location);
    Proposal(const Proposal& proposal);
};

class KDE : public Distribution
{
private:
    long size;
    float min, max, * mu, * sig, * w, scaling, total;
    VSLStreamStatePtr stream;

    std::map<float, float> S;
    std::list<float> slope, interception, area;

private:
    float regEM(float& del, const std::vector<float>& X,
        float* log_lh, float* log_sig, float* maxll, 
        float* maxlsig, float* density, float* psigd, float* ones);

private:
    void Build(long N);

    void Insert(const Proposal& proposal);
    void Update(const Proposal& proposal, Proposal& previous);
    //
    Proposal Sample();

public:
    virtual void density(float* X, float* Y, long N);
    virtual void sampling(float* X, long N);

public:
    KDE(std::vector<float>& X, const float& min, const float& max);
    virtual ~KDE();
};
#endif