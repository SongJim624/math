//Accrodint to the Simulation(Fifth edition)
//Sheldon M.Ross
#include "Sampling.h"

/*
* basic idea of Alias Sampling
* form the alias table
*/

Alias::Alias(std::vector<float>& events)
{
    std::list<Event> queue{};

    while(queue.size() != 0)
    {
        std::sort(queue.begin(), queue.end(), Compare);

        auto& larger = sort.begin();
        auto& smaller = sort.rbegin();

        if(smaller.mass == 1)
        {
            events.insert(events.end(), queue.begin(), queue.end());
            return;
        }

        larger.mass -= 1 - smaller.mass;
        
        if(larger.mass == 1)
        {
        //    larger.alias = larger.event; // this should be the default setting.
            events.push_back(larger);
            queue.pop_front();
        }

        smaller.alias = larger.event;

        events.push_back(smaller);
        queue.pop_back();
    }
}

void Alias(float * Y, long size)
{
    vsRngUniform(, size, 0, 1);

    for(long i = 0; i < N; ++i)
    {
        long location = rand() % N;
        Y[i] = Y[i] < events[location].mass ? events[location].event : events[location].alias;
    }
}