#include "Metropolis Hastings.h"

void Gamma_Metropolis_Hasting(size_t length, double * samples)
{
//    float * U = new float[size];
//    vsRngUniform(, , U, 0, 1);

    proposal->sample(length, sapmles);

    Y[0] = X0;
    for(long i = 1; i < size; ++i)
    {
        float rho = powf((Y[i] - Y[i - 1]) * expf((Y[i - 1] - Y[i]) / alpha), alpha - floor(alpha));

        if(fminf(rho, 1) > U[i])
            Y[i] = Y[i - 1];
    }

    delete[] U;
    delete[] F;
    delete[] G;
    return;
}