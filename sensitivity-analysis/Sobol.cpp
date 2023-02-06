#include "Sobol.h"

std::vector<float> Sobol(Function func, std::vector<Distribution*>& parameter)
{
    size_t n = parameter.size();

    std::vector<std::vector<float>(N)> A(n);
    std::vector<std::vector<float>(N)> B(n);

    for(size_t i = 0; i < n; ++i)
    {
        A[i] = sampling->sampling(N, parameter[i]);
    }
}