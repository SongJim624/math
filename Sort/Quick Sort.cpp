#include <random>

void Quick(double * &x, size_t &begin, size_t &end)
{
    while(begin < end)
    {
        size_t q = Partition(x, begin, end);
        Quick(x, begin, q - 1);
        begin = q + 1;
    }
}

size_t Partition(double * &x, const size_t &begin, const size_t &end)
{
    int r = (size_t)rand() % end + begin;

    double temp = x[r];

    double inter;
    size_t i = begin - 1;
    for(size_t j = begin; j < end - 1; ++j)
    {
        if(x[j] <= temp)
        {
            i += 1;
            inter = x[i];
            x[i] = x[j];
            x[j] = inter;
        }
    }

    i += 1;
    x[end] = x[i];
    x[i] = temp;

    return i;
}

void Quick(double * &x, const size_t &begin, const size_t &end)
{

}