#include <map>
#include <array>
#include <iostream>
#include <fstream>

#ifndef _LIKEHOOD_
#define _LIKEHOOD_
class Likehood
{
public:
    virtual float likehood(float * parameters, long N) = 0;
};

class AquaCrop_Likehood : public Likehood
{
private:
    std::array<std::map<long, Distribution, 4>> observations;

    void Creator();
    void Reader();

public:
    AquaCrop_Likehood(const char * file);
    ~AquaCrop_Likehood();

public:
    virtual float likehood(float * parameters, long N);
};
#endif // !_LIKEHOOD_