//Accrodint to the Simulation(Fifth edition)
//Sheldon M.Ross
#include "Sampling.h"

void Alias(float * &x, float * &mass, const size_t &num, const size_t &num_out, float * &rngs)
{
//This is suitable for the vector generation    
    float * mass_a = new float[num];

    std::vector<size_t> A(0);
    std::vector<size_t> B(0);

    std::vector<size_t> Q1(0);
    std::vector<size_t> Q2(0);

    std::vector<float> mass_f(0);

    float m;
    for(size_t i = 0; i < num; ++i)
    {
        m = mass[i] * num;
        mass[i] = m;
        if(m > 1)
        {
            A.push_back(i);
            continue;
        }

        if(m < 1)
        {
            B.push_back(i);
            continue;
        }

        if(m == 1)
        {
            Q1.push_back(i);
            Q2.push_back(i);
            mass_f.push_back(1.0);
        }
    }

    size_t big, small;
    while(A.size() != 0)
    {
        big = A.size() - 1;
        small = B.size() - 1;

        Q1.push_back(small);
        Q2.push_back(big);
        mass_f.push_back(mass[small]);

        B.pop_back();

        mass[big] -= (1 - mass[small]);
        
        if(mass[big] < 1.0)
        {
            B.push_back(big);
            A.pop_back();
        }
        else if(mass[big] == 1.0)
        {
            Q1.push_back(big);
            Q2.push_back(big);
            mass_f.push_back(1.0);
        }
    }

    for(size_t i = 0; i < num_out; ++i)
    {
        size_t index = rand() % num;
        if((rand() % (float)RAND_MAX) > mass_f[index])
        {
            rngs[i] = Q2[index];
        }
        else
        {
            rngs[i] = Q1[index];
        }
    }
}
