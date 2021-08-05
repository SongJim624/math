#include "Matrix.h"

Sparse& operator += (Sparse& A, const Sparse& B)
{
    for (auto r = B.data.begin(); r != B.data.end(); ++r)
    {
        auto iter = A.data.find(r->first);

        if (iter == A.data.end())
        {
            A.data.insert(*r);
        }
        else
        {
            for (auto c = r->second.begin(); c != r->second.end(); ++c)
            {
                auto it = iter->second.find(c->first);

                if(it == iter->second.end())
                {
                    iter->second.insert(*c);
                }
                else
                {
                    it->second += c->second;
                }
            }
        }
    }

    return A;
}

Sparse& operator -= (Sparse& A, const Sparse& B)
{
    for (auto r = B.data.begin(); r != B.data.end(); ++r)
    {
        auto iter = A.data.find(r->first);

        if (iter == A.data.end())
        {
            A.data.insert(*r);
        }
        else
        {
            for (auto c = r->second.begin(); c != r->second.end(); ++c)
            {
                auto it = iter->second.find(c->first);

                if (it == iter->second.end())
                {
                    iter->second.insert(*c);
                }
                else
                {
                    it->second -= c->second;
                }
            }
        }
    }

    return A;
}

IMatrix& operator += (IMatrix& A, const Sparse& B)
{
    for (auto r : B.data)
    {
        for (auto c : r.second)
        {
            A(r.first, c.first) += c.second;
        }
    }

    return A;
}

IMatrix& operator -= (IMatrix& A, const Sparse& B)
{
    for (auto r : B.data)
    {
        for (auto c : r.second)
        {
            A(r.first, c.first) -= c.second;
        }
    }

    return A;
}