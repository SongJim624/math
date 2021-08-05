#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <mkl.h>
#include <time.h>
#include "Distribution.h"

#ifndef _EMPIRICAL_
#define _EMPIRICAL_
struct Alias
{
    float origin, alias;
    float mass;
};

class Empirical : public Distribution
{
private:
    VSLStreamStatePtr stream;
    std::map<float, float> table;
    std::vector<Alias> events;

public:
    Empirical(std::vector<float>& data);
    ~Empirical();

public:
    void density(float * X, float * Y, long N);
    //Alias method is used
    void sampling(float * X, long N);
};
#endif