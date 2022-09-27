#include <stddef.h>
#include <math.h>

//This is the orgnial thinkings.
float Determinant(const float ** A, size_t num)
{
    float res = 0;
    
    if(num == 1)
    {
        res += A[0][0];
    }
    else
    {
        for(size_t i = 0; i < num; ++i)
        {
            float ** A1 = new float * [num - 1];
            size_t index = 0;
            for(size_t j = 0; j < num; ++j)
            {
                A1[index] = (A[j] + 1);
                ++index;
                if(j == i){continue;}
            }

            res += i % 2 ?
                A[i][0] * Determinant(A, num - 1) :
                -A[i][0] * Determinant(A, num - 1); 

            delete[] A1;
        }
    }

    return res;
}

//Matlab uses the LU split and then calculate.