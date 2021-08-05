#include "../Distribution.h"
#include <limits>
#include "windows.h"

/*
#ifndef _Density_
#define _Density_
template<typename T>
class Density
{
private:
    size_t row, column;
    std::vector<T> data;

private:
    bool Check(const Density& A)
    {
        if (row != A.row) { MessageBox("row not equal!"); return false; }
        if (column != A.column) {
            MessageBox("column not equal!"); return false;
        }
        return true;
    }

    bool Check(const size_t&r, const size_t& c)
    {
        if (r > row) { MessageBox("row exceeds!"); return false; }
        if (c > column) { MessageBox("column exceeds!"); return false; }
        return true;
    }

public:

    Density& operator + (const Density& A)
    {
        if (Check(A))
        {
            for (size_t i = 0; i < data.size(); ++i)
            {
                data[i] += A.data[i];
            }
        }

        return *this;
    }

    Density& operator - (const Density& A)
    {
        if (Check(A))
        {
            for (size_t i = 0 i < data.size(); ++i)
            {
                data[i] -= A.data[i];
            }
        }
        return *this;
    }

//row domain, begin with 1
    T& operator ()(const size_t& r, const size_t& c)
    {
        if (Check(r, c)) { return data[column * (r - 1) + (c - 1)]; }
    }

    std::vector<T>& Row(const size_t& r)
    {
        if (Check(r, 0))
        {
            return 
        }
    }
//

};
#endif // !_Matrix_
*/

#ifndef _KDE_
#define _KDE_
template<typename T>
class KDE : public Distribution<T>
{
private:
    float * mu, sig, w;
    long size;
    float MAX, MIN, scaling;

//    std::vector<T> mu, sig, w;
//    T MAX, MIN, scaling;

private:
    virtual void density(float * X, float * Y, long size);
    virtual void distribution(float * X, float * Y, long size);

    std::vector<T> pdf(const std::vector<T>& X);
    T pdf(const T& X);
    std::vector<T> cdf(const std::vector<T>& X);

public:
    KDE (const std::vector<T>& X);
};
#endif