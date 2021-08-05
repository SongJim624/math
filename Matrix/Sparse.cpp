#include "Matrix.h"
Sparse::Sparse(const long& row, const long& column)
    : IMatrix(row, column)
{}

float& Sparse::operator()(long r, long c)
{
    auto i = data.find(r);
    if (i == data.end())
    {
        data.insert(std::pair<long, std::map<long, float>>
        {r, std::map<long, float>{std::pair<long, float>{c, 0}}});

        return data[r].begin()->second;
    }

    auto j = i->second.find(c);
    if (j == i->second.end())
    {
        i->second.insert(std::pair<long, float>{c, 0});
        return i->second[c];
    }

    return j->second;
}

float Sparse::operator()(long r, long c) const
{
    auto i = data.find(r);
    if (i == data.end())
    {
        return 0;
    }

    auto j = i->second.find(c);
    if (j == i->second.end())
    {
        return 0;
    }

    return j->second;
}