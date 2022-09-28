#include "math.h"

void Cholevsky(float ** &A, float * b, const size_t &num)
{
    A[0][0] = sqrt(A[0][0]);
    for(size_t i = 0; i < num; ++i)
    {
        A[i][0] /= A[0][0];
    }

    for(size_t k = 1; k < num; ++k)
    {
        for(size_t i = 0; i < k; ++i)
        {
            A[k][k] -= A[k][i] * A[k][i];
        }
        A[k][k] = sqrt(A[k][k]);

        for(size_t i = 0; i < num; ++i)
        {
            for(size_t j = 0; j < k; ++j)
            {
                A[i][k] -= A[i][j] * A[k][j];
            }

            A[i][k] /= A[k][k]; 
        }
    }

    b[0] /= A[0][0];
    for(size_t i = 1; i < num; ++i)
    {
        for(size_t s = 0; s < i; ++s)
        {
            b[i] -= b[s] * A[i][s];
        }
        b[i] /= A[i][i];
    }

    b[num - 1] = b[num - 1] / A[num - 1][num - 1];
    for (long i = num - 2; i !< 0; --i)
    {
        for (size_t s = 0; s < i; ++s)
        {
            b[i] -= b[s] * A[s][i];
        }
        b[i] /= A[i][i];
    }
}