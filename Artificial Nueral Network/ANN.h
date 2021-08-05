#include <math.h>

class Ueuron
{

};

class Node
{
private:    
    Node * next;
    size_t num;
public:
    Node()
    {
        next = nullptr;
        num = 0;
    }

    void Add(Node * &node)
    {
        num += 1;
        Node * inter = new Node [num];
        for(size_t i = 0; i < num - 2; ++i)
        {
            inter[i] = next[i];
        }
        inter[num - 1] = node; 
        delete[] next;
        next = inter;
    }
    
    ~Node()
    {
        if(num == 0)
        {
            return;
        }

        for(size_t i = 0; i < num; ++i)
        {
            delete next[i];
        }
        delete[] next;
    }
};


class ANN
{
protected:
//Network structure    

public:    
    virtual void Train(float ** &x, float ** &y, const size_t &num) = 0;
    virtual void Simulation(float * &x, float * &y) = 0;
};

class BP : public ANN
{
private:
    BP(const size_t &input, const size_t &hidden, const size_t &output);
    virtual void Train(float ** &x, float ** &y, const size_t &num);
    virtual void Simulation(float * &x, float * &y);
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