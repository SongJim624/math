#include <list>
#include <utility>
#include <map>
#include <vector>
#include <mkl.h>
#include <time.h>
#include <algorithm>

struct Proposal
{
    float sample;
    float proposal;
    float target;
    long location;

    Proposal(float& sample, float& proposal, float& target, long& location);
    Proposal(const Proposal& proposal);
};

class IA2RMS
{
private:
/*
* S is the set of support points
* the map is used to store the table of X value and the correspoding target density
*/
    float max, min;

    std::map<float, float> S;
    std::list<float> slope, interception;

private:
//Build the proposal
    void Build(long N);
    
    void Insert(const Proposal& proposal);
    void Update(const Proposal& proposal, Proposal& previous);
//
    Proposal Sample();
    void density(float  * X, float * Y, long N);

public:
    void sampling(float * X, long N);

};