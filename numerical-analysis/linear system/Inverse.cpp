#include "math.h"

//This method is from the Numerical Analysis
//David kincaid Third version page 157

void Inverse(const float * matrix, float * inversion, size_t size)
{
    
}
void Inverse(float ** A, size_t num)
{
    size_t n = 20;
    for(size_t i =0; i < num; ++i)
    {
        for(size_t j = 0; j < num; ++j)
        {
            A[i][j] = -A[i][j];
        }
    }

    for(size_t i = 0; i < num; ++i)
    {
        A[i][i] += 1;
    }

    float ** Temp = new float *[num];
    for(size_t i = 0; i < num; ++i)
    {
        Temp[i] = new float[num];
    }
    float sum;

    for(size_t i = 0; i < n; ++i)
    {
        for(size_t i = 0; i < num; ++i)
        {
            for(size_t j = 0; j < num; ++j)
            {
                for(size_t k = 0; k < num; ++k)                
                {

                }
                Temp[i][j] = sum;
            }
        }    
    }

    for(size_t i = 0; i < num; ++i)
    {
        delete[] Temp[i];
    }
    delete Temp;
}