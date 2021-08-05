class Distribution
{
public:
    virtual void density(float * X, float * Y, long N) = 0;
    virtual void sampling(float * X, long N) = 0;
    virtual ~Distribution(){};
};