#include <vector>
#include <list>
#include <algorithm>

struct Event
{
    float event;
    float mass;
    float alias;
};

bool Compare(Event& A, Event& B)
{
    return A.mass > B.mass;
}

class Alias
{
private:
    Event* events;

public:
 // the constructor needs more consideration
    Alias(std::vector<float> samples);
};