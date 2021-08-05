#include "Matrix.h"

bool operator == (const IMatrix& A, const IMatrix& B)
{
    if(A.row != B.row)
        return false;
    
    if(A.column != B.column)
        return false;

    for(long i = 0; i < A.row; ++i)
    {
        for(long j =0; j < A.column; ++j)
        {
            if(A(i, j) != B(i, j))
                return false;
        }
    }

    return true;
}

IMatrix& Transposite(IMatrix& A)
{
    for(long i = 0; i < A.row; ++i)
    {
        
    }

}
