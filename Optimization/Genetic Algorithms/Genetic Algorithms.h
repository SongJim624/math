#include <math.h>
#include <vector>

typedef float * (* fitness)(float *);
typedef float (* constraints)(float *); 

class GA
{
//The cross over setions

//The mutation sections

//The select sections

    void GetProblem(int &Fitness, size_t &num_dec, size_t &num_obj,
        float * &Lower, float * &Upper, size_t * Integer, int &Constraint);
}