#include "math.h"

class ANN
{
protected:
    inline float Sigmoid(const float &x)
    {
        return  1 / (1 - exp(-x));
    }

    inline float Tan(const float &x)
    {
        return 
    }

public:    
    virtual void Train(float ** &x, float ** &y, const size_t &num) = 0;
    virtual void Simulation(float * &x, float * &y) = 0;
};

class BP : public ANN
{
private:
    BP(const size_t &input, const size_t &hidden, const size_t &output);
    void Train(float ** &x, float ** &y, const size_t &num);
    void Simulation(float * &x, float * &y);
public:
    static ANN * Create(const size_t &input, const size_t &hidden, const size_t &output)
    {
        return new BP(input, hidden, output);
    }
};

class RBF : public ANN
{
private:
    RBF(const size_t &input, const size_t &hidden, const size_t &output);
    void Train(float ** &x, float ** &y, const size_t &num);
    void Simulation(float * &x, float * &y);
public:
    static ANN * Create(const size_t &input, const size_t &hidden, const size_t &output)
    {
        return new RBF(input, hidden, output);
    }
};