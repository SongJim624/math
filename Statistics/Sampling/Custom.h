#include "Sampling.h"

template<typename T>
class Custom : public PDF<T>, public CDF<T>, public Sample<T>
{

public:
    Custom(void* func);
};