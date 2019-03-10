//This is for the one dimension condition

#include <time.h>

typedef float(*pf) (float);

float Direct()
{
//The Uniform random    
    float r = (float) rand() / (float(RAND_MAX));

}

//Box - Muller method
//Here I use the method form the book
//page 64, for its one output
float Normal()
{
    while(true)
    {
        float r1 = -log((float) rand() / (float(RAND_MAX)));
        float r2 = -log((float) rand() / (float(RAND_MAX)));
        
        float inter = 1 - r1;
        if(r2 > inter * inter/ 2.0)
        {
            float r = (float)rand() / (float)RAND_MAX;
            return r < 0.5 ? r1 : -r1;
        }
    }   
}

float Exponential(float lamada)
{
    float r = (float)rand() / (float)(RAND_MAX); 

//Since 1 - r and r are both of the uniform distribution
//the orginally  1 - r is replaced by r to save time.
    return -log(r) / lamada;
}










float Gamma(float alpha, float beta)
{
    float r = 0;
    for(int i = 0; i < (int)alpha, i++)
    {
        r += -log((float)rand() / (float)(RAND_MAX));
    }

    return r * beta;
}

float Benomial(int a, int b)
{
    float numerator = 0;
    float denominator = 0;

    float r;
    for(int i = 0; i < a + b; i++)
    {
        r = -log((float)rand() / (float)RAND_MAX);

        denominator += r;


        if(i >= a)
        {
            continue;
        }
        else
        {
            numerator += r;
        }
        
    }
    return numerator / denominator;
}