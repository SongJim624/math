#pragma once
#include <time.h>
#include <string>
class Sampling
{




public:
    Sampling()
    {
        srand((unsigned)time(NULL));
    }

    void random();
}