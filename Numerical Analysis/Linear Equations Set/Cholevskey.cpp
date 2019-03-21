#include "math.h"

void Cholevsky(float ** &A, float * &b, const size_t &num)
{
    A[0][0] = sqrt(A[0][0]);
    for(size_t i = 0; i < num; ++i)
    {
        A[i][0] = A[0][i] / A[0][0];
    }

    float sum;
    for(size_t k = 1; k < num; ++k)
    {
        sum = 0; 
        for(size_t s = 0; s < k; ++s)
        {
            sum += A[k][s] * A[k][s];
        }
        A[k][k] = sqrt(A[k][k] - sum);

        for(size_t i = 0; i < num; ++i)
        {
            sum = 0;
            for(size_t s = 0; s < k; ++s)
            {
                sum += A[i][s] * A[s][k];
            }

            A[i][k] = (A[i][k] -sum) / A[k][k]; 
        }
    }

    if(b == nullptr)
    {
        return;
    }

    b[0] = b[0] / A[0][0];
    for(size_t i = 1; i < num; ++i)
    {
        sum = 0;
        for(size_t s = 0; s < i; ++s)
        {
            sum += b[s] * A[i][s];
        }
        b[i] = (b[i] - sum) / A[i][i];
    }

    b[num - 1] = b[num - 1] / A[num - 1][num - 1];
    size_t i = num - 2;
    while(i < num)
    {
        sum = 0;
        for(size_t s = 0; s < i; ++s)
        {
            sum += b[s] * A[s][i]; 
        }
        b[i] = (b[i] - sum) / A[i][i];
        
        --i;
    }
}