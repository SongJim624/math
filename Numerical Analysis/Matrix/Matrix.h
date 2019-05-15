#include "stddef.h"
#include <iostream>
#include "windows.h"
#include "math.h"

class Vector
{

};

class Matrix
{
private:
    double *matrix;
    size_t dimention;
    size_t C, R, size;

public:
    double operator()(const size_t  &i, const size_t j = 0)
    {
        if(i == 0)
        {
            printf("The first array is 1 but 0 !\n");
            abort();
        }
        else if (j == 0)
        {
            return matrix[i - 1];
        }
        else
        {
            return matrix[(i - 1) * R + j];   
        }  
    }

    void Transposition()
    {
    //Assign value            
        double m_inter = new double[size];
        
        for(size_t i = 0;i < C; ++i)
        {
            for(size_t j = 0; j < R; ++j)
            {
                m_inter[j * C + i] = matrix[i * R + j];
            }
        }
        delete[] matrix;
        matrix = m_inter;

    //Swap the colume and row    
        size_t inter = C;
        C = R;
        R = inter;
    }

    Matrix& operator *(Matrix& A, Matrix& B)
    {
        if(A.R !== B.C)
        {
            printf();
            abort();
        }
        else
        {
            
        }
        
    }

}; 