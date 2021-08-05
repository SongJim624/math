#include "Metropolis Hastings.h"

void Independent_Metropolis_Hasting(float * Y, long size)
{
    float * U = new float[size];
    proposal->sample(Y, size);

    vsRngUniform(, , U, 0, 1);

    float * F = new float[2];
    float * G = new float[2];

    Y[0] = X0;
    for(long i = 1; i < size; ++i)
    {
        d->density(Y + i - 1, F, 2);
        p->density(Y + i - 1, G, 2);

        if(fminf(F[1] * G[0] / (F[0] * G[1]), 1) > U[i])
            Y[i] = Y[i - 1];
    }

    delete[] U;
    delete[] F;
    delete[] G;
    return;
}