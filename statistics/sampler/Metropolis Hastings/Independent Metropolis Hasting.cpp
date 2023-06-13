#include "Metropolis Hastings.h"

void Independent_Metropolis_Hasting(size_t length, double * samples)
{
    float * U = new float[size];
    proposal->sample(Y, size);

    vsRngUniform(, , U, 0, 1);

    double F[2], G[2];

    Y[0] = X0;
    for(size_t i = 1; i < size; ++i)
    {
        d->density(samples + i - 1, F, 2);
        p->density(samples + i - 1, G, 2);

        if(fminf(F[1] * G[0] / (F[0] * G[1]), 1) > U[i])
            Y[i] = Y[i - 1];
    }

    delete[] U;
    return;
}