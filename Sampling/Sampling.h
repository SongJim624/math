#pragma once
#include <time.h>
#include <string>
#include <vector>
#include <math.h>

typedef float (* pf)(float);

class Sampling
{




public:
    Sampling()
    {
        srand((unsigned)time(NULL));
    }

    void random();
}