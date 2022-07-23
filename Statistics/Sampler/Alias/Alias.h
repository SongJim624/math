#include <vector>
#include <list>
#include <algorithm>
/*
bool Compare(Event& A, Event& B)
{
    return A.mass > B.mass;
}
*/
template<typename T>
class Alias
{
private:
    struct Event
    {
        float event, mass, alias;
    };

    Event * events;

public:
 // the constructor needs more consideration
    Alias(const std::vector<T>& samples);
};