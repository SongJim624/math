#include "Equation.h"

float Bisection(std::fucntion<float(float)>& f, float left, float right, float tol)
{
    float left_ = f(left);
    float right_ =f(right);

    while(true)
    {
        assert(left_ * right_ < 0, "solution not in the interval");

        float middle = 0.5f * (left + right);
        float value = f(middle);

        if(fabsf(value) < tol)
        {
            return middle;
        }

        if(value * left < 0)
        {
            right = middle;
            right_ = value;
        }
        else
        {
            left = middle;
            left_ = value;
        }
    }
}