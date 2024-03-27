#include "Empirical.h"

class Compare
{
public:
    bool operator () (const Alias& A, const Alias& B)
    {
        return A.mass > B.mass;
    }
};

Empirical::Empirical(std::vector<float>& data)
    : stream(nullptr)
{
    vslNewStream(&stream, VSL_BRNG_MCG31, (unsigned long)time(NULL));
    std::list<Alias> queue{};

    std::sort(data.begin(), data.end());
    auto iter = data.begin();
    table[*iter] = 1;

    while (++iter != data.end())
    {
        auto last = table.rbegin();
        *iter == last->first ? last->second++ : table[*iter] = 1;
    }

    for (auto& it : table)
    {
        it.second /= data.size();
        queue.push_back(Alias{ it.first,it.first,it.second * table.size() });
    }

    while (queue.size() != 0)
    {
        queue.sort(Compare());

        auto larger = queue.begin();
        auto smaller = queue.rbegin();

        if (smaller->mass == 1)
        {
            events.insert(events.end(), queue.begin(), queue.end());
            return;
        }

        larger->mass -= 1 - smaller->mass;
        smaller->alias = larger->origin;

        if (larger->mass == 1)
        {
            events.push_back(*larger);
            queue.pop_front();
        }

        events.push_back(*smaller);
        queue.pop_back();
    }
}

Empirical::~Empirical()
{
    vslDeleteStream(&stream);
}

void Empirical::mass(float* X, float* Y, long N)
{
    for (long i = 0; i < N; ++i)
    {
        auto iter = table.find(X[i]);

        Y[i] = iter == table.end() ? 0 : iter->second;
    }
}

void Empirical::sampling(long* X, long N)
{
    vsRngUniform(VSL_RNG_METHOD_UNIFORM_STD_ACCURATE, stream, N, X, 0, 1);

    for (long i = 0; i < N; ++i)
    {
        long location = rand() % events.size();
        X[i] = X[i] < events[location].mass ? events[location].origin : events[location].alias;
    }
}