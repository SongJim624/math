inline float Norm(float * &a, float * &b, size_t &num)
{
    float min = +INFINITY;
    float inter;
    for(size_t i = 0; i < num ; ++i)
    {
        inter = absf(a[i] - b[i]);
        if(inter < min)
        {
            min = inter;
        }
    }

    return min;
}